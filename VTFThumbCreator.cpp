/*    VTFThumbCreator - show thumbnails for Valve Texture Format files in KDE
 *    Copyright (C) 2014  Mathias Panzenböck
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "VTFThumbCreator.h"

#include <QString>
#include <QImage>

#include <VTFFile.h>
#include <VTFLib.h>

bool VTFThumbCreator::create(const QString &path, int width, int height, QImage &image) {
    if (width <= 0 || height <= 0) {
        return false;
    }

    VTFLib::CVTFFile file;

    if (!file.Load(path.toLocal8Bit().constData())) {
        qDebug("%s", VTFLib::LastError.Get());
        return false;
    }

    // find most appropriate image
    const vlByte *frame = 0;
    VTFImageFormat srcformat;

    vlUInt vwidth  = file.GetThumbnailWidth();
    vlUInt vheight = file.GetThumbnailHeight();
//    vlUInt vdepth  = file.GetDepth();

    if (file.GetHasThumbnail() && (vwidth >= (vlUInt)width || vheight >= (vlUInt)height)) {
        // get image from thumbnail
        srcformat = file.GetThumbnailFormat();
        frame = file.GetThumbnailData();
    }
    else {
        // search best fitting mipmap
        srcformat = file.GetFormat();

        vwidth  = file.GetWidth();
        vheight = file.GetHeight();
/*
        vlUInt level   = file.GetMipmapCount();
        vlUInt mwidth  = 0;
        vlUInt mheight = 0;
        vlUInt mdepth  = 0;

        while (level > 0) {
            VTFLib::CVTFFile::ComputeMipmapDimensions(vwidth, vheight, vdepth, level, mwidth, mheight, mdepth);
            if (mwidth >= (vlUInt)width || mheight >= (vlUInt)height)
                break;
            -- level;
        }

        vwidth  = mwidth;
        vheight = mheight;
        vdepth  = mdepth;

        frame = file.GetData(file.GetFrameCount() - 1, 0, 0, level);
*/
        frame = file.GetData(file.GetFrameCount() - 1, 0, 0, 0);
    }

    if (!frame) {
        return false;
    }

    QImage::Format qformat;
    VTFImageFormat dstformat;
    if (VTFLib::CVTFFile::GetImageFormatInfo(srcformat).uiAlphaBitsPerPixel > 0) {
        qformat   = QImage::Format_ARGB32;
        dstformat = IMAGE_FORMAT_RGBA8888;
    }
    else {
        qformat   = QImage::Format_RGB888;
        dstformat = IMAGE_FORMAT_RGB888;
    }

    // ensure image has the requested dimensions
    if ((vlUInt)image.width() != (vlUInt)vwidth || (vlUInt)image.height() != (vlUInt)vheight || image.format() != qformat) {
        image = QImage(vwidth, vheight, qformat);
        if (image.isNull()) {
            return false;
        }
    }

    uchar *bits = image.bits();
    if (!VTFLib::CVTFFile::Convert(frame, bits, vwidth, vheight, srcformat, dstformat)) {
        qDebug("%s", VTFLib::LastError.Get());
        return false;
    }

    if (dstformat == IMAGE_FORMAT_RGBA8888) {
        // For some reason the colors are swapped around this way.
        // I don't know if the error is in VTFLib, Qt or my usage of either.
        for (size_t i = 0, n = vwidth * vheight * 4; i < n; i += 4) {
            uchar b = bits[i + 0];
            uchar r = bits[i + 2];

            bits[i + 0] = r;
            bits[i + 2] = b;
        }
    }

    return true;
}

ThumbCreator::Flags VTFThumbCreator::flags() const {
    return None;
}

ThumbCreator *new_creator() {
    return new VTFThumbCreator();
}
