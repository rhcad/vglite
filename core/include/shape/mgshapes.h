//! \file mgshapes.h
//! \brief 定义图形列表类 MgShapes
// Copyright (c) 2004-2012, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#ifndef __GEOMETRY_MGSHAPES_H_
#define __GEOMETRY_MGSHAPES_H_

#include "mgbox.h"
#include "mgobject.h"

class MgShape;
struct MgStorage;
class MgShapes;
class GiGraphics;
class GiContext;

//! 图形列表类
/*! \ingroup GEOM_SHAPE
    \see MgShapeIterator, MgShape::createShape()
*/
class MgShapes : public MgObject
{
public:
    //! 返回本对象的类型
    static int Type() { return 1; }
    
    //! 复制出一个新图形列表对象
    MgShapes* cloneShapes() const { return (MgShapes*)clone(); }
    
    //! 创建图形列表
    static MgShapes* create(MgObject* owner = NULL, int index = -1);

#ifndef SWIG
    MgShape* getFirstShape(void*& it) const;
    MgShape* getNextShape(void*& it) const;
    void freeIterator(void*& it);
    typedef bool (*Filter)(const MgShape*);
#endif

    int getShapeCount() const;
    MgShape* getHeadShape() const;
    MgShape* getLastShape() const;
    MgShape* findShape(int sid) const;
    MgShape* findShapeByTag(int tag) const;
    MgShape* findShapeByType(int type) const;
    Box2d getExtent() const;
    
    MgShape* hitTest(const Box2d& limits, Point2d& nearpt
#ifndef SWIG
        , int* segment = NULL, Filter filter = NULL) const;
#else
        ) const;
#endif
    
    int draw(GiGraphics& gs, const GiContext *ctx = NULL) const;
    int dyndraw(int mode, GiGraphics& gs, const GiContext *ctx, int segment) const;

    bool save(MgStorage* s, int startIndex = 0) const;
    bool load(MgStorage* s, bool addOnly = false);
    
    //! 删除所有图形
    void clear();

    //! 复制(深拷贝)每一个图形
    void copyShapes(const MgShapes* src);
    
    //! 复制出新图形并添加到图形列表中
    MgShape* addShape(const MgShape& src);

    //! 添加一个指定类型的新图形
    MgShape* addShapeByType(int type);
    
    //! 移除一个图形，由调用者删除图形对象
    MgShape* removeShape(int sid, bool skipLockedShape = true);

    //! 将一个图形移到另一个图形列表
    MgShape* moveTo(int sid, MgShapes* dest);

    //! 将所有图形移到另一个图形列表
    void moveAllShapesTo(MgShapes* dest);
    
    //! 移动图形到最后，以便显示在最前面
    bool bringToFront(int sid);

    //! 返回拥有者对象
    MgObject* getOwner() const;
    
public:
    virtual MgObject* clone() const;
    virtual void copy(const MgObject& src);
    virtual void release();
    virtual bool equals(const MgObject& src) const;
    virtual int getType() const { return Type(); }
    virtual bool isKindOf(int type) const { return type == Type(); }

protected:
    MgShapes(MgObject* owner, int index);
    virtual ~MgShapes();
    
    virtual bool saveExtra(MgStorage* s) const { return !!s; }
    virtual bool loadExtra(MgStorage* s) { return !!s; }
    
    struct I;
    I*  im;
};

//! 遍历图形的辅助类
/*! \ingroup GEOM_SHAPE
*/
class MgShapeIterator
{
public:
    MgShapeIterator(const MgShapes* shapes) : _s(shapes), _it(NULL) {}
    ~MgShapeIterator() { freeIterator(); }

    MgShape* getFirstShape() { return _s->getFirstShape(_it); }
    MgShape* getNextShape() { return _s->getNextShape(_it); }
    void freeIterator() { _s->getNextShape(_it); }

private:
    MgShapeIterator();
    const MgShapes* _s;
    void* _it;
};

#endif // __GEOMETRY_MGSHAPES_H_
