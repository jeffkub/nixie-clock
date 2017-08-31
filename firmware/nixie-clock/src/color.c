/*******************************************************************************
MIT License

Copyright (c) 2017 Jeff Kubascik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

/* Includes *******************************************************************/
#include "color.h"

#include <math.h>

#include "globals.h"


/* Private definitions ********************************************************/


/* Private variables **********************************************************/


/* Private function prototypes ************************************************/


/* Private function definitions ***********************************************/


/* Public function definitions ************************************************/
void color_hsvToRgb(const hsv_t * hsv, rgb_t * rgb)
{
    float chrom;
    float huep;
    float x;
    float m;

    chrom = hsv->val * hsv->sat;

    huep = hsv->hue / 60.0f;

    x = chrom * (1.0f - fabsf(fmodf(huep, 2.0f) - 1.0f));

    if(0.0f <= huep && huep < 1.0f)
    {
        rgb->red = chrom;
        rgb->grn = x;
        rgb->blu = 0.0f;
    }
    else if(1.0f <= huep && huep < 2.0f)
    {
        rgb->red = x;
        rgb->grn = chrom;
        rgb->blu = 0.0f;
    }
    else if(2.0f <= huep && huep < 3.0f)
    {
        rgb->red = 0.0f;
        rgb->grn = chrom;
        rgb->blu = x;
    }
    else if(3.0f <= huep && huep < 4.0f)
    {
        rgb->red = 0.0f;
        rgb->grn = x;
        rgb->blu = chrom;
    }
    else if(4.0f <= huep && huep < 5.0f)
    {
        rgb->red = x;
        rgb->grn = 0.0f;
        rgb->blu = chrom;
    }
    else if(5.0f <= huep && huep < 6.0f)
    {
        rgb->red = chrom;
        rgb->grn = 0.0f;
        rgb->blu = x;
    }

    m = hsv->val - chrom;

    rgb->red += m;
    rgb->grn += m;
    rgb->blu += m;

    return;
}

void color_ledAdjust(rgb_t * rgb)
{
    rgb->red = rgb->red * rgb->red;
    rgb->grn = rgb->grn * rgb->grn;
    rgb->blu = rgb->blu * rgb->blu;

    return;
}
