#include "apt_dat.h"

APT_dat::APT_dat(QString zpath, QString yfhomedir)
{
    homeDir = yfhomedir.trimmed();
    aptdatFilePath = zpath.trimmed();
    decompressedFilePath = homeDir + "/tmp_apt_dat";
    aptcache = homeDir + "/apts.cache";
    rwscache = homeDir + "/rws.cache";

    isDecompressed = false;
}

void APT_dat::read()
{
    QFile inFile(aptdatFilePath);
    inFile.open(QIODevice::ReadOnly);
    QByteArray ba = inFile.readAll();

    QFile file(decompressedFilePath);
    if(file.open(QIODevice::WriteOnly)){
        QDataStream out(&file);
        out << gUncompress(ba);
        file.close();
        inFile.close();
    }
    else
    {
        qWarning("Can't open file %s for write: %s", file.fileName().toStdString().data(), file.errorString().toStdString().data());
    }
}

QHash<QString, QStringList> APT_dat::getAirports()
{
    QHash<QString, QStringList> airports;
    airports.begin();
    QFile file(aptcache);
    if(file.open(QIODevice::ReadWrite|QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QStringList items = in.readLine().split("|");
            if(items.count()<3){
                qFatal("Cache file %s not valid", aptcache.toStdString().data());
                return airports;
            }
            airports.insert(items[0], QStringList() << items[1] << items[2]);
        }
        airports.end();
        file.close();
    }else{
        qFatal("Cannot open file %s: Error %s", file.fileName().toStdString().data(), file.errorString().toStdString().data());
        return airports;
    }
    return airports;
}

QList<Runway *> APT_dat::getRunwaysByAirport(QString key)
{
    QList<Runway *> runways;
    int counter = 0;
    runways.begin();
    bool foundFirstMatch = false;
    QFile file(rwscache);
    if(file.open(QIODevice::ReadWrite|QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QStringList items = in.readLine().split("|");
            if(items.count()<7){
                qFatal("Cache file %s not valid", rwscache.toStdString().data());
                return runways;
            }
            if(items[0].trimmed().compare(key)==0){
                runways.insert(counter++, new Runway(items[1].trimmed(),
                                                     items[2].trimmed(),
                                                     items[3].trimmed(),
                                                     items[4].trimmed(),
                                                     items[5].trimmed(),
                                                     items[6].trimmed()));
                if(!foundFirstMatch)
                    foundFirstMatch = true;
            }
            if(foundFirstMatch && (items[0].trimmed().compare(key)!=0))
            {
                break;
            }
        }
        runways.end();
        file.close();
    }else{
        qFatal("Cannot open file %s: Error %s", file.fileName().toStdString().data(), file.errorString().toStdString().data());
        return runways;
    }
    return runways;
}


bool APT_dat::create_cache(QStringList all_airports_dir)
{
    allAirportsDir = all_airports_dir;

    if(!isDecompressed)
    {
        if(!QFile::exists(decompressedFilePath))
            read();
        isDecompressed = true;
    }

    QFile cache_apt(aptcache);

    if(cache_apt.exists() && !QFile::remove(aptcache))
    {
        qFatal("Cannot remove file %s. Cache NOT built!", aptcache.toStdString().data());
        return false;
    }

    QFile cache_rws(rwscache);
    if(cache_rws.exists() && !QFile::remove(rwscache))
    {
        qFatal("Cannot remove file %s. Cache NOT built!", rwscache.toStdString().data());
        return false;
    }

    QFile file(decompressedFilePath);
    QString token;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&file);
        in.readLine(); in.readLine(); // skip two header lines containing IBM/DOS and licenses
        while(!in.atEnd())
        {
            token = in.readLine();
            if(token.trimmed().compare("")==0)
                continue; // skip empty lines
            QStringList lineItems = token.split(" ",QString::SkipEmptyParts);
            if(lineItems.count()>0){
                switch(lineItems[0].toInt())
                {
                    case APTDAT_AIRPORT:
                    case APTDAT_SEAPLANE_BASE:
                    case APTDAT_HELIPORT:
                        parseAirportLine(lineItems);
                        break;
                    case APTDAT_RUNWAY:
                    case APTDAT_RUNWAY_850:
                    case APTDAT_RUNWAY_WATER:
                    case APTDAT_RUNWAY_HELIPAD:
                        parseRunwayLine(lineItems);
                        break;
                }
            }
        }
    }
    return true;
}

bool APT_dat::aptcache_exists()
{
    QFile cache(aptcache);
    return cache.exists();
}

bool APT_dat::rwscache_exists()
{
    QFile cache(rwscache);
    return cache.exists();
}

bool APT_dat::addAirport()
{
    QString airportDirectory = "";
    Airport *ap;
    foreach(QString airportDir, allAirportsDir)
    {
        ap = new Airport(airportDir,lastAirportName);
        QDir dir(ap->getAirportDirPath());
        if(dir.exists())
        {
            airportDirectory = QString(ap->getAirportDirPath());
            break;
        }
    }

    QFile file(aptcache);
    if(file.open(QIODevice::Append|QIODevice::Text)){
        QTextStream out(&file);
        out << lastAirportName << "|" << lastAirportDescription << "|" << airportDirectory << endl;
        file.close();
    }else{
        qFatal("Cannot open file %s: Error %s", file.fileName().toStdString().data(), file.errorString().toStdString().data());
        return false;
    }
    return true;
}

bool APT_dat::addRunway()
{
    QFile file(rwscache);
    if(file.open(QIODevice::Append|QIODevice::Text)){
        QTextStream out(&file);
        out << lastAirportName << "|" << lastRunway->toString() << endl;
        file.close();
    }else{
        qFatal("Cannot open file %s: Error %s", file.fileName().toStdString().data(), file.errorString().toStdString().data());
        return false;
    }
    return true;
}

void APT_dat::parseAirportLine(QStringList items)
{
    /*
      0 -> APTDAT_AIRPORT/SEAPLANE_BASE/HELIPORT
      4 -> ICAO
      */
    QString icao = items[4].trimmed();
    QString airportDescription;
    for(int i=0;i<5;i++)
        items.pop_front();
    airportDescription = items.join(" ");

    lastAirportName = icao;
    lastAirportDescription = airportDescription;
    addAirport();
}

void APT_dat::parseRunwayLine(QStringList items)
{
    /*
    ' id --> what it is
    ' 1  --> latitude
    ' 2  --> longitude
    ' 3  --> runway number
    ' 4  --> runway heading (true, not magnetic)
    ' 5  --> runway segment length in feet
    ' SKIPPED: two fields to understand (see http://data.x-plane.com/file_specs/Apt810.htm)
    ' 8  --> runway/taxiway segment width in feet
    ' 9  --> runway/taxiway segment lighting codes
    ' 10 --> runway surface code
    ' 11 --> runway shoulder code
    */

    lastRunway = new Runway(items[3].trimmed(),
                            items[1].trimmed(),
                            items[2].trimmed(),
                            items[4].trimmed(),
                            items[10].trimmed(),
                            items[11].trimmed()
                            );

    addRunway();

    /*lastRunwayList.insertMulti(icao, new Runway(items[3].trimmed(),
                                       items[1].trimmed(),
                                       items[2].trimmed(),
                                       items[4].trimmed(),
                                       items[10].trimmed(),
                                       items[11].trimmed()
                                       ));*/

}

QByteArray APT_dat::gUncompress(const QByteArray &data)
{
    if (data.size() <= 4) {
        qWarning("gUncompress: Input data is truncated");
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15 +  32); // gzip decoding
    if (ret != Z_OK)
        return QByteArray();

    // run inflate()
    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     // and fall through
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}

