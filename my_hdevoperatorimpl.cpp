// HDevEngine/C++ example implementations for HDevelop's display operators.
//
//   This file contains a simple implementation for single-threaded
//   applications and a more complex thread save implementation
//
// (C) 2007-2020 MVTec Software GmbH
//

#include "my_hdevoperatorimpl.h"


#define HCkDev(DEV_OP)                                                        \
  try                                                                         \
  {                                                                           \
    DEV_OP;                                                                   \
    return H_MSG_TRUE;                                                        \
  }                                                                           \
  catch (HException & exc)                                                    \
  {                                                                           \
    return exc.ErrorCode();                                                   \
  }


using namespace HalconCpp;

/*****************************************************************************
 * MyHDevOperatorImpl
 *****************************************************************************
 * This is a simple thread save implementation of the HDevelop's display
 * operators.
 *****************************************************************************/


size_t MyHDevOperatorImpl::GetCount() const
{
  return GetWinIds().GetCount();
}


void MyHDevOperatorImpl::AddWindow(const HTuple& id)
{
  GetWinIds().AddWindow(id);
}


HTuple MyHDevOperatorImpl::PopWindow()
{
  return GetWinIds().PopWindow();
}


Hlong MyHDevOperatorImpl::SetWindow(const HTuple& id)
{
  return GetWinIds().SetWindow(id);
}


// overloaded display operators
int MyHDevOperatorImpl::DevOpenWindow(const HTuple& row, const HTuple& col,
                                      const HTuple& width,
                                      const HTuple& height,
                                      const HTuple& background, HTuple* win_id)
{
  (void)background;
  try
  {
    OpenWindow(row, col, width, height, 0, "visible", "", win_id);
    GetWinIds().AddWindow((*win_id)[0]);
    return H_MSG_TRUE;
  }
  catch (HException& exc)
  {
    return exc.ErrorCode();
  }
}


int MyHDevOperatorImpl::DevSetWindowExtents(const HTuple& row,
                                            const HTuple& col,
                                            const HTuple& width,
                                            const HTuple& height)
{
  HCkDev(SetWindowExtents(GetCurrentWindow(), row, col, width, height));
}


int MyHDevOperatorImpl::DevSetPart(const HTuple& row1, const HTuple& col1,
                                   const HTuple& row2, const HTuple& col2)
{
  HCkDev(SetPart(GetCurrentWindow(), row1, col1, row2, col2));
}


int MyHDevOperatorImpl::DevSetWindow(const HTuple& win_id)
{
  return (int)(GetWinIds().SetWindow(win_id));
}


int MyHDevOperatorImpl::DevGetWindow(HTuple* win_id)
{
  if (win_id)
    *win_id = HTuple(GetCurrentWindow());
  return H_MSG_TRUE;
}


int MyHDevOperatorImpl::DevClearWindow()
{
  HCkDev(ClearWindow(GetCurrentWindow()));
}


int MyHDevOperatorImpl::DevCloseWindow()
{
  // no return, no exception: do not throw an exception if no window is open
  try
  {
    HTuple win_id = GetWinIds().PopWindow();
    if (win_id.Length() > 0 && win_id.TupleIsValidHandle().L())
      CloseWindow(win_id);
  }
  catch (HException&)
  {
  }
  return H_MSG_TRUE;
}


int MyHDevOperatorImpl::DevDisplay(const HObject& obj)
{
  HCkDev(DispObj(obj, GetCurrentWindow()));
}


int MyHDevOperatorImpl::DevDispText(const HTuple& string,
                                    const HTuple& coordSystem,
                                    const HTuple& row, const HTuple& column,
                                    const HTuple& color,
                                    const HTuple& genParamName,
                                    const HTuple& genParamValue)
{
  HCkDev(DispText(GetCurrentWindow(), string, coordSystem, row, column, color,
                  genParamName, genParamValue));
}


int MyHDevOperatorImpl::DevSetDraw(const HTuple& draw)
{
  HCkDev(SetDraw(GetCurrentWindow(), draw));
}


int MyHDevOperatorImpl::DevSetContourStyle(const HTuple& draw)
{
  HCkDev(SetContourStyle(GetCurrentWindow(), draw));
}


int MyHDevOperatorImpl::DevSetShape(const HTuple& shape)
{
  HCkDev(SetShape(GetCurrentWindow(), shape));
}


int MyHDevOperatorImpl::DevSetColor(const HTuple& color)
{
  HCkDev(SetColor(GetCurrentWindow(), color));
}


int MyHDevOperatorImpl::DevSetColored(const HTuple& colored)
{
  HCkDev(SetColored(GetCurrentWindow(), colored));
}


int MyHDevOperatorImpl::DevSetLut(const HTuple& lut)
{
  HCkDev(SetLut(GetCurrentWindow(), lut));
}


int MyHDevOperatorImpl::DevSetPaint(const HTuple& paint)
{
  HCkDev(SetPaint(GetCurrentWindow(), paint));
}


int MyHDevOperatorImpl::DevSetLineWidth(const HTuple& width)
{
  HCkDev(SetLineWidth(GetCurrentWindow(), width));
}


HTuple MyHDevOperatorImpl::GetCurrentWindow() const
{
  return GetWinIds().GetCurrentWindow();
}


// handles of all opened windows, the last one is the active one
MyHDevOperatorImpl::WinIdContainer& MyHDevOperatorImpl::GetWinIds()
{
  mMutex.Lock();
  WinIdContainer& win_ids = mWinIdsMap[GetThreadId()];
  mMutex.Unlock();
  return win_ids;
}


const MyHDevOperatorImpl::WinIdContainer& MyHDevOperatorImpl::GetWinIds() const
{
  return const_cast<MyHDevOperatorImpl*>(this)->GetWinIds();
}


/*****************************************************************************
 * MyWinIdContainer
 *****************************************************************************
 * Class that collects all open windows and the currently active window of one
 * thread
 *****************************************************************************/

MyHDevOperatorImpl::WinIdContainer::WinIdContainer() {}


MyHDevOperatorImpl::WinIdContainer::~WinIdContainer()
{
  for (ListType::iterator it = mIds.begin(); it != mIds.end(); ++it)
    delete (*it);
}


HTuple MyHDevOperatorImpl::WinIdContainer::GetCurrentWindow() const
{
  if (mIds.empty())
    return HTuple();
  return HTuple(*(mIds.back()));
}


void MyHDevOperatorImpl::WinIdContainer::AddWindow(const HTuple& id)
{
  mIds.push_back(new HTuple(id));
}


HTuple MyHDevOperatorImpl::WinIdContainer::PopWindow()
{
  if (mIds.empty())
    return HTuple();
  HTuple* id = mIds.back();
  HTuple  win_id(*id);
  delete id;
  mIds.pop_back();
  return win_id;
}


Hlong MyHDevOperatorImpl::WinIdContainer::SetWindow(const HTuple& id)
{
  ListType::iterator it = Find(id);
  // handle not found
  if (it == mIds.end())
    return H_MSG_FALSE;
  // put handle at end of list
  mIds.erase(it);
  mIds.push_back(new HTuple(id));
  return H_MSG_TRUE;
}


MyHDevOperatorImpl::WinIdContainer::ListType::iterator
MyHDevOperatorImpl::WinIdContainer::Find(const HTuple& id)
{
  for (ListType::iterator it = mIds.begin(); it != mIds.end(); ++it)
    if (*(*it) == id)
      return it;
  return mIds.end();
}
