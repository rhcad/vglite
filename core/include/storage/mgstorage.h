//! \file mgstorage.h
//! \brief 定义图形存取接口 MgStorage
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#ifndef TOUCHVG_MGSTORAGE_H_
#define TOUCHVG_MGSTORAGE_H_

//! 图形存取接口
/*! \ingroup CORE_STORAGE
    \interface MgStorage
*/
struct MgStorage
{
    //! 通知一个节点开始读取或读取完成
    /*! 一个节点会调用两次本函数，节点可包含多个键值对，可嵌套子节点。
        \param name 节点名称前缀，与index一起形成实际存储的节点名称
        \param index 小于0表示只有这一个子节点，否则表示可能有多个同级同名节点并指定子节点序号
        \param ended true表示本次调用时是节点结束，该节点的数据已读取完成；
                     false表示是节点开始，此后将读取该节点的数据
        \return 是否有此节点
    */
    virtual bool readNode(const char* name, int index, bool ended) = 0;

    //! 通知一个节点开始添加或添加完成
    /*! 一个节点会调用两次本函数，节点可包含多个键值对，可嵌套子节点。
        \param name 节点名称前缀，与index一起形成实际存储的节点名称
        \param index 小于0表示只有这一个子节点，否则表示可能有多个同级同名节点并指定子节点序号
        \param ended true表示本次调用时是节点结束，该节点的数据已添加完成；
                     false表示是节点开始，此后将添加该节点的数据
        \return 是否添加成功
    */
    virtual bool writeNode(const char* name, int index, bool ended) = 0;
    
    //! 给定字段名称(常量)，取出一个布尔值
    virtual bool readBool(const char* name, bool defvalue) = 0;
    //! 给定字段名称(常量)，取出一个浮点数的值
    virtual float readFloat(const char* name, float defvalue) = 0;

    //! 添加一个给定字段名称(常量)的布尔值
    virtual void writeBool(const char* name, bool value) = 0;
    //! 添加一个给定字段名称(常量)的浮点数的值
    virtual void writeFloat(const char* name, float value) = 0;
    //! 添加一个给定字段名称(常量)的字符串内容(有零结束符)
    virtual void writeString(const char* name, const char* value) = 0;
    
#ifndef SWIG
    //! 给定字段名称(常量)，取出浮点数数组. 传入缓冲为空时返回所需个数
    virtual int readFloatArray(const char* name, float* values, int count) = 0;
    //! 给定字段名称(常量)，取出字符串内容，不含零结束符. 传入缓冲为空时返回所需字符个数
    virtual int readString(const char* name, char* value, int count) = 0;
    //! 添加一个给定字段名称(常量)的浮点数数组
    virtual void writeFloatArray(const char* name, const float* values, int count) = 0;

    //! 给定字段名称(常量)，取出一个单字节整数的值
    virtual char  readInt8(const char* name, char defvalue) {
        return (char)readInt(name, defvalue); }
    //! 给定字段名称(常量)，取出一个双字节整数的值
    virtual short readInt16(const char* name, short defvalue) {
        return (short)readInt(name, defvalue); }
    //! 给定字段名称(常量)，取出一个长整数的值
    virtual long readInt32(const char* name, long defvalue) {
        return (long)readInt(name, defvalue); }
    //! 给定字段名称(常量)，取出一个单字节整数的值
    virtual unsigned char  readUInt8(const char* name, unsigned char defvalue) {
        return (unsigned char)readInt(name, defvalue); }
    //! 给定字段名称(常量)，取出一个双字节整数的值
    virtual unsigned short readUInt16(const char* name, unsigned short defvalue) {
        return (unsigned short)readInt(name, defvalue); }
    //! 给定字段名称(常量)，取出一个长整数的值
    virtual unsigned long readUInt32(const char* name, unsigned long defvalue) {
        return (unsigned long)readInt(name, defvalue); }

    //! 添加一个给定字段名称(常量)的单字节整数的值
    virtual void writeInt8(const char* name, char value) { writeInt(name, value); }
    //! 添加一个给定字段名称(常量)的双字节整数的值
    virtual void writeInt16(const char* name, short value) { writeInt(name, value); }
    //! 添加一个给定字段名称(常量)的长整数的值
    virtual void writeInt32(const char* name, long value) { writeInt(name, value); }
    //! 添加一个给定字段名称(常量)的单字节整数的值
    virtual void writeUInt8(const char* name, unsigned char value) { writeUInt(name, value); }
    //! 添加一个给定字段名称(常量)的双字节整数的值
    virtual void writeUInt16(const char* name, unsigned short value) { writeUInt(name, value); }
    //! 添加一个给定字段名称(常量)的长整数的值
    virtual void writeUInt32(const char* name, unsigned long value) { writeUInt(name, value); }
#endif

    //! 给定字段名称(常量)，取出一个整数的值
    virtual int readInt(const char* name, int defvalue) { return name ? defvalue : defvalue; }
    //! 添加一个给定字段名称(常量)的有符号整数的值
    virtual void writeInt(const char* name, int value) { if (name && value) value++; }
    //! 添加一个给定字段名称(常量)的无符号整数的值
    virtual void writeUInt(const char* name, int value) { if (name && value) value++; }

    //! 设置读写错误描述文字，总是返回false
    virtual bool setError(const char*) { return false; }
};

#endif // TOUCHVG_MGSTORAGE_H_
