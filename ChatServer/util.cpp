#include "util.h"


Util::Util()
{

}

bool Util::isPicture(QString filepath)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filepath);
    return mime.name().startsWith("image/");
}

