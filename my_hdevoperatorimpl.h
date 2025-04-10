// HDevEngine/C++ example implementations for HDevelop's display operators.
//
//   This file contains a simple but thread save implementation for
//   single or multithreaded applications
//
// (C) 2007-2020 MVTec Software GmbH
//

#ifndef MY_HDEV_OPERATOR_IMPL_H
#define MY_HDEV_OPERATOR_IMPL_H

#ifdef _WIN32
// disable warning C4786: symbol greater than 255 character,
#pragma warning(disable : 4786)
#endif

#ifdef __APPLE__
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#    include <HDevEngineCpp/HDevEngineCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#    include <HDevEngineCppxl/HDevEngineCpp.h>
#  endif
#else
#  include "halconcpp/HalconCpp.h"
#  include "hdevengine/HDevEngineCpp.h"
#endif

#include <list>
#include <map>

#include "my_thread_impl.h"



/*****************************************************************************
 * MyHDevOperatorImpl
 *****************************************************************************
 * This is a simple thread save implementation of the HDevelop's display
 * operators.
 *****************************************************************************/
class MyHDevOperatorImpl : public HDevEngineCpp::HDevOperatorImplCpp
{
public:

  HalconCpp::HTuple GetCurrentWindow()  const;
  size_t            GetCount()          const;
  void              AddWindow(const HalconCpp::HTuple& id);
  HalconCpp::HTuple PopWindow();
  Hlong             SetWindow(const HalconCpp::HTuple& id);

  // overloaded display operators
  virtual int DevOpenWindow(const HalconCpp::HTuple& row,
                            const HalconCpp::HTuple& col,
                            const HalconCpp::HTuple& width,
                            const HalconCpp::HTuple& height,
                            const HalconCpp::HTuple& background,
                            HalconCpp::HTuple* win_id);
  virtual int DevSetWindowExtents(const HalconCpp::HTuple& row,
                                  const HalconCpp::HTuple& col,
                                  const HalconCpp::HTuple& width,
                                  const HalconCpp::HTuple& height);
  virtual int DevSetPart(const HalconCpp::HTuple& row1,
                         const HalconCpp::HTuple& col1,
                         const HalconCpp::HTuple& row2,
                         const HalconCpp::HTuple& col2);
  virtual int DevSetWindow(const HalconCpp::HTuple& win_id);
  virtual int DevGetWindow(HalconCpp::HTuple* win_id);
  virtual int DevClearWindow();
  virtual int DevCloseWindow();
  virtual int DevDisplay(const HalconCpp::HObject& obj);
  virtual int DevDispText(const HalconCpp::HTuple& string,
                          const HalconCpp::HTuple& coordSystem,
                          const HalconCpp::HTuple& row,
                          const HalconCpp::HTuple& column,
                          const HalconCpp::HTuple& color,
                          const HalconCpp::HTuple& GenParamName,
                          const HalconCpp::HTuple& GenParamValue);
  virtual int DevSetDraw(const HalconCpp::HTuple& draw);
  virtual int DevSetContourStyle(const HalconCpp::HTuple& style);
  virtual int DevSetShape(const HalconCpp::HTuple& shape);
  virtual int DevSetColor(const HalconCpp::HTuple& color);
  virtual int DevSetColored(const HalconCpp::HTuple& colored);
  virtual int DevSetLut(const HalconCpp::HTuple& lut);
  virtual int DevSetPaint(const HalconCpp::HTuple& paint);
  virtual int DevSetLineWidth(const HalconCpp::HTuple& width);

protected:

  /***************************************************************************
   * MyWinIdContainer
   *   Class that collects all open windows and the currently active window
   *   of one thread
   ***************************************************************************/
  class WinIdContainer
  {
  public:
    typedef std::list<HalconCpp::HTuple*> ListType;

    WinIdContainer();
    ~WinIdContainer();
    HalconCpp::HTuple  GetCurrentWindow() const;
    size_t GetCount() const
    {
      return mIds.size();
    }
    void   AddWindow(const HalconCpp::HTuple& id);
    HalconCpp::HTuple  PopWindow();
    Hlong  SetWindow(const HalconCpp::HTuple& id);
    ListType::iterator Find(const HalconCpp::HTuple& id);

  protected:
    ListType  mIds;
  };

  // handles of all opened windows, the last one is the active one
  WinIdContainer&       GetWinIds();
  const WinIdContainer& GetWinIds() const;

  // map that is used for attaching the window handles to the appropriate
  // threads
  std::map<Hlong,WinIdContainer>  mWinIdsMap;
  MyMutexImpl                     mMutex;
};


#endif // #ifndef MY_HDEV_OPERATOR_IMPL_H

