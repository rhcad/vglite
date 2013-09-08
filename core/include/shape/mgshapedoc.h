//! \file mgshapedoc.h
//! \brief 定义图形文档 MgShapeDoc
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#ifndef __GEOMETRY_SHAPEDOC_H_
#define __GEOMETRY_SHAPEDOC_H_

#include "mgshapes.h"

class MgLayer;

//! 读写锁定数据类
/*! \ingroup CORE_SHAPE
 */
class MgLockRW
{
public:
    MgLockRW();
    bool lockData(bool forWrite, int timeout = 200);
    long unlockData(bool forWrite);
    
    bool firstLocked() const;
    bool lockedForRead() const;
    bool lockedForWrite() const;
    
    int getEditFlags() const { return _editFlags; }
    void setEditFlags(int flags) {
        _editFlags = flags ? (_editFlags | flags) : 0;
    }
    
private:
    volatile long _counts[3];
    int     _editFlags;
};

//! 图形文档
/*! \ingroup CORE_SHAPE
 */
class MgShapeDoc : public MgObject
{
public:
    //! 返回本对象的类型
    static int Type() { return 8; }
    
    //! 复制出一个新图形文档对象
    MgShapeDoc* cloneDoc() const { return (MgShapeDoc*)clone(); }
    
    //! 创建图形文档对象
    static MgShapeDoc* create();
    
    //! 增加引用
    void addRef();
    
    //! 保存图形
    bool save(MgStorage* s, int startIndex = 0) const;
    
    //! 加载图形
    bool load(MgStorage* s, bool addOnly = false);
    
    //! 删除所有图形
    void clear();
    
    //! 释放临时数据内存
    void clearCachedData();
    
    //! 显示所有图形
    int draw(GiGraphics& gs) const;
    
    //! 返回图形范围
    Box2d getExtent() const;
    
    //! 返回图形总数
    int getShapeCount() const;
    
    //! 返回当前图形列表（图层或复合图形）
    MgShapes* getCurrentShapes() const;

    //! 设置当前图形列表（图层或复合图形）
    bool setCurrentShapes(MgShapes* shapes);

    //! 切换图层，最多10个图层
    bool switchLayer(int index);
    
    //! 返回新图形的图形属性
    GiContext* context();
    
    //! 模型变换矩阵
    Matrix2d& modelTransform();
    
    //! 得到页面范围的世界坐标
    Box2d getPageRectW() const;
    
    //! 得到显示比例
    float getViewScale() const;
    
    //! 设置页面范围的世界坐标
    void setPageRectW(const Box2d& rectW, float viewScale);
    
    //! 返回改变计数
    int getChangeCount() const;
    
    //! 改变完成后更新改变计数
    void afterChanged();
    
    //! 得到锁定数据对象以便读写锁定
    virtual MgLockRW* getLockData();
    
public:
    virtual MgObject* clone() const;
    virtual void copy(const MgObject& src);
    virtual void release();
    virtual bool equals(const MgObject& src) const;
    virtual int getType() const { return Type(); }
    virtual bool isKindOf(int type) const { return type == Type(); }
    
protected:
    MgShapeDoc();
    virtual ~MgShapeDoc();
    
private:
    struct Impl;
    Impl*  im;
};

//! 图形列表锁定辅助类
/*! \ingroup CORE_SHAPE
 */
class MgShapesLock
{
    int     _mode;
public:
    MgShapeDoc*   doc;
    
    enum { Unknown = 0x100, ReadOnly = 0,
        Add = 0x1, Remove = 0x2, Edit = 0x4, Load = 0x8 };
    MgShapesLock(int flags, MgShapeDoc* doc, int timeout = 200);
    ~MgShapesLock();
    
    bool locked() const;
    static bool lockedForRead(MgShapeDoc* doc);
    static bool lockedForWrite(MgShapeDoc* doc);
    
    int getEditFlags() const { return doc->getLockData()->getEditFlags(); }
    void resetEditFlags() { doc->getLockData()->setEditFlags(0); }
    
#ifndef SWIG
    typedef void (*ShapesLocked)(MgShapeDoc* doc, void* obj, bool locked);
    static void registerObserver(ShapesLocked func, void* obj);
    static void unregisterObserver(ShapesLocked func, void* obj);
#endif
};

//! 动态图形锁定辅助类
/*! \ingroup CORE_SHAPE
 */
class MgDynShapeLock
{
    int     _mode;
public:
    MgDynShapeLock(bool forWrite = true, int timeout = 200);
    ~MgDynShapeLock();
    
    bool locked() const;
    static bool lockedForRead();
    static bool lockedForWrite();
};

#endif // __GEOMETRY_SHAPEDOC_H_
