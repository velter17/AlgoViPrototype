/**
 * Project   Graviz
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
    QString to_search_in = "<div class=\"input\">";
    QString to_search_out = "<div class=\"output\">";
    QString from_str = "<pre>";
    QString to_str = "</pre>";
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

    while(1)
    {
        int posin = html.indexOf(to_search_in, prev);
        if(posin == -1)
            break;
        qDebug () << "posin = " << posin;
        int lin = html.indexOf(from_str, posin);
        int rin = html.indexOf(to_str, posin);
        qDebug () << html.mid(lin, rin-lin);

        int posout = html.indexOf(to_search_out, posin);
        if(posout == -1)
            break;
        qDebug () << "posout = " << posout;
        int lout = html.indexOf(from_str, posout);
        int rout = html.indexOf(to_str, posout);
        qDebug () << html.mid(lout, rout-lout);

        mTests.push_back(STest(extractData(html.mid(lin, rin-lin)), extractData(html.mid(lout, rout-lout))));

        prev = posout;
    }
    return 0;
}

} // namespace NCommand
