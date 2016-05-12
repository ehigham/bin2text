/*    strings.h
 *
 *    defines common strings used by bin2text
 *
 *    This file is part of bin2text.
 *
 *    bin2text is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at youroption) any later version.
 *
 *    bin2text is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with bin2text.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    (C) 2016, Thomas Legris, Edmund Higham
**/

#ifndef BIN2TEXT_RESOURCE_STRINGS_H
#define BIN2TEXT_RESOURCE_STRINGS_H

struct output_filenames
{
    const char * const out1_name;
    const char * const out2_name;
    const char * const out3_name;
    const char * const out4_name;
    const char * const out5_name; 
};

static const struct output_filenames output_files;

#endif // BIN2TEXT_RESOURCE_STRINGS_H

