#include "apt_dat.h"

APT_dat::APT_dat(QString zpath, QString dpath)
{
    aptdatFilePath = zpath.trimmed();
    decompressedFilePath = dpath.trimmed();
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

void APT_dat::parse_apt_dat()
{
    if(!isDecompressed)
    {
        if(!QFile::exists(decompressedFilePath))
            read();
        isDecompressed = true;
    }

    airportNameList.begin();
    runwayList.begin();

    QFile file(decompressedFilePath);
    QString token;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&file);
        in.readLine(); in.readLine(); // skip two header lines containing IBM/DOS and licenses
        QString airportName; // using as key for runwaylist
        while(!in.atEnd())
        {
            token = in.readLine();
            if(token.trimmed().compare("")==0)
                continue; // skip empty lines
            QStringList lineItems = token.split(" ");
            if(lineItems.count()>0){
                switch(lineItems[0].toInt())
                {
                    case APTDAT_AIRPORT:
                    case APTDAT_SEAPLANE_BASE:
                    case APTDAT_HELIPORT:
                        airportName = parseAirportLine(lineItems);
                        break;
                    case APTDAT_RUNWAY:
                    case APTDAT_RUNWAY_850:
                    case APTDAT_RUNWAY_WATER:
                    case APTDAT_RUNWAY_HELIPAD:
                        parseRunwayLine(airportName, lineItems);
                        break;
                }
            }
        }
        file.close();
        airportNameList.end();
    }
}

QString APT_dat::parseAirportLine(QStringList items)
{
    /*
      0 -> APTDAT_AIRPORT/SEAPLANE_BASE/HELIPORT
      4 -> ICAO
      */
    QString icao = items[4].trimmed();
    for(int i=0;i<5;i++)
        items.pop_front();
    airportNameList.insert(icao,items.join(" "));
    return icao;
}

void APT_dat::parseRunwayLine(QString icao, QStringList items)
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

    runwayList.insert(icao, new Runway(items[3].trimmed(),
                                       items[1].trimmed(),
                                       items[2].trimmed(),
                                       items[4].trimmed(),
                                       items[10].trimmed(),
                                       items[11].trimmed()
                                       ));

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

