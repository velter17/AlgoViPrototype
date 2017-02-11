/**
 * Project   AlgoVi
 *
 * @file     CCodeforcesParser.cpp
 * @author   Dmytro Sadovyi
 * @date     10.12.2016
 */

#include "../CCodeforcesParser.h"
#include <QEventLoop>

namespace NCommand
{

CCodeforcesParser::CCodeforcesParser(const QStringList &args)
    : IProblemParser(args)
{

}

int CCodeforcesParser::parse(const QString& path)
{
    QUrl url = path;
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply *reply(manager.get(request));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QString html = reply->readAll();
//    QString to_search_in = "<div class=\"input\">";
//    QString to_search_out = "<div class=\"output\">";
    QString from_str = "pre";
    QString to_str = "/pre";
    int prev = 0;

    auto extractData = [](const QString& str){
        QString ret;
        int cnt = 0;
        for(int i = 0; i < str.length(); ++i)
        {
            if(str[i] == '<')
                ++cnt;
            else if(str[i] == '>')
                --cnt;
            else if(cnt == 0)
            {
                if(i > 0 && !ret.isEmpty() && str[i-1] == '>')
                    ret.append('\n');
                ret.append(str[i]);
            }
        }
        return ret;
    };

    int state = 0;
    // 0 - init
    // 1 - inside open tag
    // 2 - outside open tag
    // 3 - inside close tag

    QString tagStr;
    int openTagIn[2], closeTagOut;
    int counter = 0;
    for(int i = 0; i < html.length(); ++i)
    {
        if(html[i] == '<')
        {
            if(state == 0)
            {
                openTagIn[counter] = i;
                state = 1;
            }
            else if(state == 2)
            {
                state = 3;
            }
        }
        else if(html[i] == '>')
        {
            if(state == 1)
            {
                if(tagStr == from_str)
                {
                    state = 2;
                }
                else
                    state = 0;
            }
            else if(state == 3)
            {
                if(tagStr == to_str)
                {
                    if(counter == 1)
                    {
                        mTests.push_back(STest(extractData(html.mid(openTagIn[0], closeTagOut-openTagIn[0]+1)),
                                               extractData(html.mid(openTagIn[1], i-openTagIn[1]+1))));
                        counter = 0;
                    }
                    else
                    {
                        counter++;
                        closeTagOut = i;
                    }
                    state = 0;
                }
                else
                    state = 2;
            }
            tagStr.clear();
        }
        else if(state == 1 || state == 3)
            tagStr.push_back(html[i].toLower());
    }

//    while(1)
//    {
//        int posin = prev;//html.indexOf(to_search_in, prev);
//        if(posin == -1)
//            break;
//        qDebug () << "posin = " << posin;
//        int lin = html.indexOf(from_str, posin);
//        int rin = html.indexOf(to_str, posin);
//        qDebug () << html.mid(lin, rin-lin);

//        int posout = rin+4;//html.indexOf(to_search_out, posin);
//        if(posout == -1)
//            break;
//        qDebug () << "posout = " << posout;
//        int lout = html.indexOf(from_str, posout);
//        int rout = html.indexOf(to_str, posout);
//        qDebug () << html.mid(lout, rout-lout);

//        mTests.push_back(STest(extractData(html.mid(lin, rin-lin)), extractData(html.mid(lout, rout-lout))));

//        //prev = posout;
//        prev = rout+4;
//    }
    return 0;
}

} // namespace NCommand
