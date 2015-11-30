#include "pinyinall.h"

#include <string>
#include <QFile>
#include <QIODevice>
#include <iostream>
#include <QDebug>
#include <QApplication>
#include <winsock.h>
//#include <QTextCodec>
#include <QMessageBox>

PinyinAll * PinyinAll::_pinyinall = NULL;
//Mutex PinyinAll::_mutex;
const QString chidict = "./pinyin/dict/chidict";
const QString kordict = "./pinyin/dict/kordict";
const QString japdict = "./pinyin/dict/japdict";

//UTF8	=	65001,
//        GBK		=	936,
//        SJIS	=	932
std::wstring toWideChar(const std::string& s)
{
   std::wstring result;

   int len = MultiByteToWideChar(65001, 0, s.c_str(), -1, NULL, 0);
   if (len <= 0)
       return result;

   WCHAR* src = new WCHAR[len + 1];
   if (src == NULL){
       throw std::bad_alloc();
   }
   MultiByteToWideChar(65001, 0, s.c_str(), -1, src, len);
   src[len] = 0;
   result = src;

   if(src)
   {
       delete []src;
       src = NULL;
   }
   return result;
}

void PinyinAll::create(const QString &chipath , const QString &korpath , const QString &jappath)
{
    QFile chifp(chipath);
    QFile korfp(korpath);
    QFile japfp(jappath);
    bool ret_ch = chifp.open(QIODevice::ReadOnly);
    bool ret_ko = korfp.open(QIODevice::ReadOnly);
    bool ret_ja = japfp.open(QIODevice::ReadOnly);

    if(ret_ch == 0 || ret_ko == 0 || ret_ja == 0)
    {
        QMessageBox box(QMessageBox::Warning, "错误提示", "文件打开错误\n检查文件是否存在!");
        box.setStandardButtons(QMessageBox::Ok);
        box.setButtonText(QMessageBox::Ok, "确定");
        box.exec();
        return;
    }

    qint64 low;
    int i;
    low = chifp.size();
    if (low % sizeof(CodePinyin) != 0 || low == 0)
    {

//        throw Exception(chidict + "may be wrong!" , GetLastError() , __FILE__ , __LINE__);
        qDebug() << " chidict : " << low ;
        return;
    }

    _chipnum = low / sizeof(CodePinyin);
    _chipinyin = new CodePinyin[_chipnum];
    chifp.read((char *)_chipinyin , low);
    chifp.close();
    for ( i = 0 ; i <_chipnum ; i++ )
    {
        _chipinyin[i]._code = ntohs(_chipinyin[i]._code);
    }


//    low = GetFileSize(korfp.native(),&high);
    low = korfp.size();
    if (low % sizeof(CodePinyin) != 0 || low == 0)
    {
//        throw Exception(kordict + "may be wrong!" , GetLastError() , __FILE__ , __LINE__);
        qDebug() << " kordict : " << low;
        return;
    }
    _kornum = low / sizeof(CodePinyin);
    _korpinyin = new CodePinyin[_kornum];
    korfp.read((char *)_korpinyin , low);
    korfp.close();
    for ( i = 0 ; i < _kornum ; i++)
    {
        _korpinyin[i]._code = ntohs(_korpinyin[i]._code);
    }


//    low = GetFileSize(japfp.native(),&high);
    low = japfp.size();
    if (low % sizeof(CodePinyin) != 0 || low == 0)
    {
//        throw Exception(japdict + "may be wrong!" , GetLastError() , __FILE__ , __LINE__);
        qDebug() << " japdict : " << low;
        return;
    }

    _japnum = low / sizeof(CodePinyin);
    _jappinyin = new CodePinyin[_japnum];
    japfp.read((char *)_jappinyin , low);
    japfp.close();
    for ( i = 0 ; i < _japnum ; i++)
    {
        _jappinyin[i]._code = ntohs(_jappinyin[i]._code);
    }

}

PinyinAll * PinyinAll::getInstance()
{
    if (_pinyinall == NULL)
    {
//        MutexGuard _guard(_mutex);
        if(_pinyinall == NULL)
        {
            QString runPath = QApplication::applicationDirPath();
            runPath.append("/dict");
            QString _chidict = runPath;
            QString _kordict = runPath;
            QString _japdict = runPath;
            _chidict.append("/chidict");
            _kordict.append("/kordict");
            _japdict.append("/japdict");
            _pinyinall = new PinyinAll();
            _pinyinall->create(_chidict , _kordict , _japdict);
        }
    }

    return _pinyinall;
}

std::string PinyinAll::bsearch(uint16_t wCode , CodeType type)
{
    std::string result;
    CodePinyin *pinyin;
    int num;
    if(type == CHN)//4E00-9FA5
    {
        pinyin = _chipinyin;
        num    = _chipnum;
    }
    else if (type == KOR)//AC00-D7A3
    {
        pinyin = _korpinyin;
        num    = _kornum;
    }
    else if (type == JAP)
    {
        pinyin = _jappinyin;
        num = _japnum;
    }
    else
    {
        return result;
    }

    int max = num - 1;
    int min = 0;
    int mid;
    uint16_t value;
    while(max >= min)
    {
        mid = (min + max )/ 2;
        value = pinyin[mid]._code;
        if (wCode == value)
        {
            result = pinyin[mid]._pinyin;
            break;
        }
        else if (value < wCode)
        {
            min = mid + 1;
        }
        else
        {
            max = mid - 1;
        }
    }
    return result;
}

std::string PinyinAll::toPinyin(const std::wstring lines , CodeType type  , bool lowletter , const std::string& split  )
{
    std::string result;
    std::string value;
    const wchar_t *start = lines.c_str();
    std::vector<wchar_t> alp;
    //num 1 0x30 0x39
    //english 2 0x41~0x5a 0x61~0x7a
    for (int i = 0 ; i < lines.length(); i++)
    {
        if(0x0020 <= start[i] && start[i] <= 0x007E)
        {
            if( /*(0x0030 <= start[i] && start[i] <= 0x0039) ||*/
                 (0x0041 <= start[i] && start[i] <= 0x005a) ||
                (0x0061 <= start[i] &&  start[i] <= 0x007a )
                )
            alp.push_back(start[i]);
            //not blank
            else if (!alp.empty())
            {
                    value.clear();
                    value.assign(alp.begin() , alp.end());
                    if (value != "")
                    {
                        if(result == "")
                            result = value;
                        else
                            result += split + value;
                    }
                    alp.clear();
            }
            continue;
        }

        if (!alp.empty())
        {
            value.clear();
            value.assign(alp.begin() , alp.end());
            if (value != "")
            {
                if(result == "")
                    result = value;
                else
                    result += split + value;
            }
            alp.clear();
        }


        value = bsearch(start[i] , type);
        if (value != "")
        {
            if(result == "")
                result = value;
            else
                result += split + value;
        }
    }

    if(!alp.empty())
    {
        value.clear();
        value.assign(alp.begin() , alp.end());
        if (value != "")
        {
            if(result == "")
                result = value;
            else
                result += split + value;
        }
        alp.clear();
    }

    if (lowletter)
    {
        for (int i = 0 ; i < result.length(); i++)
        {
            if (result[i] >= 'A' && result[i] <= 'Z')
            {
                result[i] = result[i] + 'a' - 'A';
            }
        }
    }

    return result;
}

std::string PinyinAll::getlanguage(const std::wstring &name)
{
    for ( int i = 0 ; i < name.length() ; ++i)
    {
        if ((name[i] >= 0x3041 && name[i] <=0x3096)||
            (name[i] >= 0x309f && name[i] <=0x30fa))
        {
            return "jan";
        }
        else if (name[i] >= 0xac00 && name[i] <= 0xd7a3)
        {
            return "kor";
        }

    }

    return "chi";
}

QString PinyinAll::toQPinyin(const QString &str,  const QString &split)
{

//    std::wstring wname = toWideChar(str.toStdString());//
//    QTextCodec *codec = QTextCodec::codecForLocale();
    std::wstring wname = str.toStdWString();
    std::string language = getlanguage(wname);
    std::string sp = split.toStdString();
    if(language == "jan")
    {
        std::string pinyin = toPinyin(wname , PinyinAll::JAP , true, sp);
        return QString(pinyin.c_str());
    }
    else if (language == "kor")
    {
        std::string pinyin = toPinyin(wname , PinyinAll::KOR , true, sp);
        return QString(pinyin.c_str());
    }
    else
    {
        std::string pinyin = toPinyin(wname , PinyinAll::CHN , true, sp);
        return QString(pinyin.c_str());
    }
}

