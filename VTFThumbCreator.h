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

#ifndef VTFTHUMBCREATOR_H
#define VTFTHUMBCREATOR_H

#include <kio/thumbcreator.h>

class VTFThumbCreator : public ThumbCreator {
public:
    bool create(const QString &path, int width, int height, QImage &image);
    Flags flags() const;
};

extern "C" {
    KDE_EXPORT ThumbCreator *new_creator();
}

#endif
