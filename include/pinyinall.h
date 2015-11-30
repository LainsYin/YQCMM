#ifndef PINYINALL_H
#define PINYINALL_H
#include <QString>
#include <iostream>
#include <cstdint>
#include <stdint.h>
#include <set>
#pragma once
//acssi 0x20~0x7E


class PinyinAll
{
public:
    enum CodeType
    {
        CHN = 0,
        KOR = 1,
        JAP = 2,
    };
private:
    struct CodePinyin{
        uint16_t	_code;
        char		_pinyin[8];
    };


    static PinyinAll *_pinyinall;
//    static Mutex	_mutex;
    CodePinyin		*_chipinyin;
    int				_chipnum;
    CodePinyin		*_korpinyin;
    int				_kornum;
    CodePinyin		*_jappinyin;
    int				_japnum;
    PinyinAll():_chipinyin(NULL),_korpinyin(NULL)
    {}
    ~PinyinAll(){ destroy();}
    void create(const QString &chipath ,
        const QString &korpath ,
        const QString &jappath);

    void destroy(){
        if(_chipinyin)
        {
            delete[] _chipinyin;
            _chipinyin = NULL;
        }
        if(_korpinyin)
        {
            delete[] _korpinyin;
            _korpinyin = NULL;
        }
        if (_jappinyin)
        {
            delete[] _korpinyin;
            _korpinyin = NULL;
        }
    }
   std::string  bsearch(uint16_t wCode , CodeType type);



public:
    static PinyinAll * getInstance();
    static void unload(){
        if(_pinyinall)
        {
//            MutexGuard guard(_mutex);b
            if(_pinyinall)
            {
                delete _pinyinall;
                _pinyinall = NULL;
            }
        }
    }


    std::string toPinyin(const std::wstring lines ,
        CodeType type = CHN ,
        bool lowletter = true,
        const std::string &split = "_" );


    std::string  getlanguage(const std::wstring &name);

    QString toQPinyin(const QString &str, const QString &split = "_");
};
#endif // PINYINALL_H
