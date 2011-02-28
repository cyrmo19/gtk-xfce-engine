/*  $Id$
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Copyright (C) 1999-2004 Olivier Fourdan (fourdan@xfce.org)
 *
 *  Portions based Thinice port by
 *                       Tim Gerla <timg@rrv.net>,
 *                       Tomas �gren <stric@ing.umu.se,
 *                       Richard Hult <rhult@codefactory.se>
 *  Portions based on Smooth theme by
 *                       Andrew Johnson <ajgenius@ajgenius.us>
 *  Portions based on IceGradient theme by
 *                       Tim Gerla <timg@means.net>
 *                       Tomas �gren <stric@ing.umu.se>
 *                       JM Perez <jose.perez@upcnet.es>
 *  Portions based on Wonderland theme by
 *                       Garrett LeSage
 *                       Alexander Larsson
 *                       Owen Taylor <otaylor@redhat.com>
 *  Portions based on Raleigh theme by
 *                       Owen Taylor <otaylor@redhat.com>
 *  Portions based on Notif theme
 *  Portions based on Notif2 theme
 *  Portions based on original GTK theme
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <math.h>
#include <gtk/gtk.h>
#include <string.h>

#include "xfce_style.h"
#include "xfce_rc_style.h"
#include "gradient_draw.h"

#define DETAIL(s)   ((detail) && (!strcmp(s, detail)))

static void xfce_style_init(XfceStyle * style);
static void xfce_style_class_init(XfceStyleClass * klass);
static GtkStyleClass *parent_class = NULL;

extern GtkStyleClass xfce_default_class;

/* Taken from raleigh theme engine */
typedef enum
{
    CHECK_LIGHT,
    CHECK_DARK,
    CHECK_BASE,
    CHECK_TEXT,
    CHECK_CROSS,
    RADIO_LIGHT,
    RADIO_DARK,
    RADIO_BASE,
    RADIO_TEXT
}
Part;

#define PART_SIZE 13

static const guchar check_light_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08,
    0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0xfc, 0x0f,
    0x00, 0x00,
};
static const guchar check_dark_bits[] = {
    0x00, 0x00, 0xfe, 0x0f, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
    0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
    0x00, 0x00,
};
static const guchar check_base_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0xfc, 0x07, 0xfc, 0x07, 0xfc, 0x07, 0xfc, 0x07,
    0xfc, 0x07, 0xfc, 0x07, 0xfc, 0x07, 0xfc, 0x07, 0xfc, 0x07, 0x00, 0x00,
    0x00, 0x00,
};
static const guchar check_text_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x0f, 0x80, 0x03, 0xc0, 0x01,
    0xe0, 0x00, 0x73, 0x00, 0x3f, 0x00, 0x3e, 0x00, 0x1c, 0x00, 0x18, 0x00,
    0x08, 0x00
};
static const guchar check_cross_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x03, 0xd8, 0x01,
    0xf8, 0x00, 0x78, 0x00, 0x38, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};
static const guchar radio_light_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x08,
    0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x04, 0x08, 0x02, 0xf0, 0x01,
    0x00, 0x00,
};
static const guchar radio_dark_bits[] = {
    0x00, 0x00, 0xf0, 0x01, 0x08, 0x02, 0x04, 0x00, 0x02, 0x00, 0x02, 0x00,
    0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};
static const guchar radio_base_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0xf0, 0x01, 0xf8, 0x03, 0xfc, 0x07, 0xfc, 0x07,
    0xfc, 0x07, 0xfc, 0x07, 0xfc, 0x07, 0xf8, 0x03, 0xf0, 0x01, 0x00, 0x00,
    0x00, 0x00,
};
static const guchar radio_text_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0xf0, 0x01,
    0xf0, 0x01, 0xf0, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

static struct
{
    const guchar *bits;
#if GTK_CHECK_VERSION(2, 2, 0)
    GList *bmap_list; /* list of GdkBitmap */
#else
    GdkBitmap *bmap;
#endif
}
parts[] =
{
    { check_light_bits, NULL },
    { check_dark_bits,  NULL },
    { check_base_bits,  NULL },
    { check_text_bits,  NULL },
    { check_cross_bits, NULL },
    { radio_light_bits, NULL },
    { radio_dark_bits,  NULL },
    { radio_base_bits,  NULL },
    { radio_text_bits,  NULL }
};

/* internal functions */
static void xfce_fill_background(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);
static void xfce_draw_grips(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gint x, gint y, gint width, gint height, GtkOrientation orientation);

static void draw_hline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x1, gint x2, gint y);
static void draw_vline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint y_1, gint y_2, gint x);
static void draw_shadow(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);

static void draw_box(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);
static void draw_check(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);
static void draw_option(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height);
static void draw_shadow_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width);
static void draw_box_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width);
static void draw_extension(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side);
static void draw_slider(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation);
static void draw_handle(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation);

static void xfce_fill_background(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    GdkRectangle clip_area;
    GradientType gradient_style = GRADIENT_VERTICAL;
    gfloat shade_start = 1.0, shade_end = 1.0;
    gboolean draw_flat = FALSE;

    if ((width == -1) && (height == -1))
        gdk_drawable_get_size(window, &width, &height);
    else if (width == -1)
        gdk_drawable_get_size(window, &width, NULL);
    else if (height == -1)
        gdk_drawable_get_size(window, NULL, &height);

    clip_area.x = x;
    clip_area.y = y;
    clip_area.width = width;
    clip_area.height = height;

    /* Spin buttons are a special case */
    if (widget && GTK_IS_SPIN_BUTTON (widget))
    {
        if (DETAIL("spinbutton_up") || DETAIL("spinbutton_down"))
        {
            if ((state_type != GTK_STATE_PRELIGHT) && (state_type != GTK_STATE_ACTIVE))
            {
                draw_flat = TRUE;
            }
        }
    }

    if ((!draw_flat) && (XFCE_RC_STYLE(style->rc_style)->gradient))
    {
        GdkGC *gc = gdk_gc_new(window);
        GdkGCValues gc_values;
        switch (XFCE_RC_STYLE(style->rc_style)->gradient_style)
        {
            case XFCE_RC_GRADIENT_HORIZONTAL:
                gradient_style = GRADIENT_HORIZONTAL;
                break;
            case XFCE_RC_GRADIENT_VERTICAL:
                gradient_style = GRADIENT_VERTICAL;
                break;
            case XFCE_RC_GRADIENT_NORTHERN_DIAGONAL:
                gradient_style = GRADIENT_NORTHERN_DIAGONAL;
                break;
            case XFCE_RC_GRADIENT_SOUTHERN_DIAGONAL:
                gradient_style = GRADIENT_SOUTHERN_DIAGONAL;
                break;
            case XFCE_RC_GRADIENT_AUTO:
            default:
                if (DETAIL("hscrollbar"))
                {
                    gradient_style = GRADIENT_VERTICAL;
                }
                else if (DETAIL("vscrollbar"))
                {
                    gradient_style = GRADIENT_HORIZONTAL;
                }
                else if (height > width)
                {
                    gradient_style = GRADIENT_HORIZONTAL;
                }
                else
                {
                    gradient_style = GRADIENT_VERTICAL;
                }
                break;
        }
        if ((state_type == GTK_STATE_ACTIVE) || (DETAIL("trough")))
        {
            shade_start = MIN (XFCE_RC_STYLE(style->rc_style)->shade_start, XFCE_RC_STYLE(style->rc_style)->shade_end);
            shade_end = MAX (XFCE_RC_STYLE(style->rc_style)->shade_start, XFCE_RC_STYLE(style->rc_style)->shade_end);
        }
        else
        {
            shade_start = XFCE_RC_STYLE(style->rc_style)->shade_start;
            shade_end = XFCE_RC_STYLE(style->rc_style)->shade_end;
        }
        gdk_gc_get_values(style->bg_gc[state_type], &gc_values);
        gdk_gc_set_function(gc, GDK_COPY);
        gdk_gc_set_line_attributes(gc, 1, GDK_LINE_SOLID, gc_values.cap_style, gc_values.join_style);

        if (area)
        {
            gdk_gc_set_clip_rectangle(gc, area);
        }
        gradient_draw_shaded(window, gc, style->colormap, area, x, y, width, height, style->bg[state_type], shade_start, shade_end, gradient_style, FALSE);
        if (area)
        {
            gdk_gc_set_clip_rectangle(gc, NULL);
        }
        g_object_unref (G_OBJECT(gc));
    }
    else
    {
        if (area)
        {
            gdk_gc_set_clip_rectangle(style->bg_gc[state_type], area);
        }
        gdk_draw_rectangle(window, style->bg_gc[state_type], TRUE, x, y, width, height);
        if (area)
        {
            gdk_gc_set_clip_rectangle(style->bg_gc[state_type], NULL);
        }
    }
}

static void xfce_draw_grip_rough (GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    gint xx, yy;
    gint xthick, ythick;
    GdkGC *light_gc, *dark_gc;
    GdkRectangle dest;

    if ((width == -1) && (height == -1))
        gdk_drawable_get_size(window, &width, &height);
    else if (width == -1)
        gdk_drawable_get_size(window, &width, NULL);
    else if (height == -1)
        gdk_drawable_get_size(window, NULL, &height);

    light_gc = style->light_gc[state_type];
    dark_gc = style->dark_gc[state_type];

    xthick = style->xthickness;
    ythick = style->ythickness;

    dest.x = x + xthick;
    dest.y = y + ythick;
    dest.width = width - (xthick * 2);
    dest.height = height - (ythick * 2);

    gdk_gc_set_clip_rectangle(light_gc, &dest);
    gdk_gc_set_clip_rectangle(dark_gc, &dest);

    if (orientation == GTK_ORIENTATION_HORIZONTAL)
    {
        if (width > 15 + xthick)
        {
            gint len = (height - 2 * (ythick + 2) > 4 ? height - 2 * (ythick + 2) :  height - 2 * ythick);
            gint delta = x + (width / 2) - 5;
            yy = y + (height - len) / 2;
            for(xx = 0; xx < 10; xx += 2)
            {
                gdk_draw_line(window, dark_gc, xx + delta, yy, xx + delta, yy + len - 1);
                gdk_draw_line(window, light_gc, xx + delta + 1, yy, xx + delta + 1, yy + len - 1);
            }
        }
    }
    else
    {
        if (height > 15 + ythick)
        {
            gint len = (width - 2 * (xthick + 2) > 4 ? width - 2 * (xthick + 2) :  width - 2 * xthick);
            gint delta = y + (height / 2) - 5;
            xx = x + (width - len) / 2;
            for(yy = 0; yy < 10; yy += 2)
            {
                gdk_draw_line(window, dark_gc, xx, yy + delta, xx + len - 1, yy + delta);
                gdk_draw_line(window, light_gc, xx, yy + delta + 1, xx + len - 1, yy + delta + 1);
            }
        }
    }

    gdk_gc_set_clip_rectangle(light_gc, NULL);
    gdk_gc_set_clip_rectangle(dark_gc, NULL);
}


static void xfce_draw_grip_slide (GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    gint xthick, ythick;
    gint gx, gy, gwidth, gheight;
    GdkGC *light_gc, *dark_gc, *mid_gc, *bg_gc;
    GdkRectangle dest;

    if ((width == -1) && (height == -1))
        gdk_drawable_get_size(window, &width, &height);
    else if (width == -1)
        gdk_drawable_get_size(window, &width, NULL);
    else if (height == -1)
        gdk_drawable_get_size(window, NULL, &height);

    light_gc = style->light_gc[state_type];
    dark_gc = style->dark_gc[state_type];
    mid_gc = style->mid_gc[state_type];
    bg_gc = style->base_gc[GTK_STATE_SELECTED];

    xthick = style->xthickness;
    ythick = style->ythickness;

    dest.x = x + xthick;
    dest.y = y + ythick;
    dest.width = width - (xthick * 2);
    dest.height = height - (ythick * 2);

    gdk_gc_set_clip_rectangle(light_gc, &dest);
    gdk_gc_set_clip_rectangle(dark_gc, &dest);
    gdk_gc_set_clip_rectangle(mid_gc, &dest);
    gdk_gc_set_clip_rectangle(bg_gc, &dest);

    gx = gy = gwidth = gheight = 0;

    if (orientation == GTK_ORIENTATION_HORIZONTAL)
    {
        gint delta = ((gint)((height - 3) / 2));
        gx = x + delta;
        gy = y + delta;
        gwidth = width - 2 * delta - 1;
        gheight = height - 2 * delta - 1;
    }
    else
    {
        gint delta = ((gint)((width - 3) / 2));
        gx = x + delta;
        gy = y + delta;
        gwidth = width - 2 * delta - 1;
        gheight = height - 2 * delta - 1;
    }

    if ((gheight > 1) && (gwidth > 1))
    {
        gdk_draw_rectangle(window, bg_gc, TRUE, gx, gy, gwidth, gheight);

        gdk_draw_line(window, dark_gc, gx, gy, gx + gwidth, gy);
        gdk_draw_line(window, dark_gc, gx, gy, gx, gy + gheight);

        gdk_draw_line(window, light_gc, gx, gy + gheight, gx + gwidth, gy + gheight);
        gdk_draw_line(window, light_gc, gx + gwidth, gy, gx + gwidth, gy + gheight);

        gdk_draw_point(window, mid_gc, gx, gy);
        gdk_draw_point(window, mid_gc, gx + gwidth, gy);
        gdk_draw_point(window, mid_gc, gx, gy + gheight);
        gdk_draw_point(window, mid_gc, gx + gwidth, gy + gheight);
    }

    gdk_gc_set_clip_rectangle(light_gc, NULL);
    gdk_gc_set_clip_rectangle(dark_gc, NULL);
    gdk_gc_set_clip_rectangle(mid_gc, NULL);
    gdk_gc_set_clip_rectangle(bg_gc, NULL);
}

static void xfce_draw_grips(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    XfceRcStyle *rc_style;

    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    rc_style = XFCE_RC_STYLE(style->rc_style);

    if (rc_style)
    {
        if (rc_style->grip_style == XFCE_RC_GRIP_ROUGH)
        {
            xfce_draw_grip_rough (style, window, state_type, area, widget, x, y, width, height, orientation);
        }
        else if (rc_style->grip_style == XFCE_RC_GRIP_SLIDE)
        {
            xfce_draw_grip_slide (style, window, state_type, area, widget, x, y, width, height, orientation);
        }
    }
}

static void draw_hline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x1, gint x2, gint y)
{
    gint thickness_light;
    gint thickness_dark;
    gint i;

    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    thickness_light = style->ythickness / 2;
    thickness_dark = style->ythickness - thickness_light;

    if (area)
    {
        gdk_gc_set_clip_rectangle(style->light_gc[state_type], area);
        gdk_gc_set_clip_rectangle(style->dark_gc[state_type], area);
    }

    for(i = 0; i < thickness_dark; i++)
    {
        gdk_draw_line(window, style->dark_gc[state_type], x2 - i - 1, y + i, x2, y + i);
        gdk_draw_line(window, style->dark_gc[state_type], x1, y + i, x2 - i - 1, y + i);
    }

    y += thickness_dark;
    for(i = 0; i < thickness_light; i++)
    {
        gdk_draw_line(window, style->light_gc[state_type], x1, y + i, x1 + thickness_light - i - 1, y + i);
        gdk_draw_line(window, style->light_gc[state_type], x1 + thickness_light - i - 1, y + i, x2, y + i);
    }

    if (area)
    {
        gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
        gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
    }
}

static void draw_vline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint y_1, gint y_2, gint x)
{
    gint thickness_light;
    gint thickness_dark;
    gint i;

    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    thickness_light = style->xthickness / 2;
    thickness_dark = style->xthickness - thickness_light;

    if (area)
    {
        gdk_gc_set_clip_rectangle(style->light_gc[state_type], area);
        gdk_gc_set_clip_rectangle(style->dark_gc[state_type], area);
    }
    for(i = 0; i < thickness_dark; i++)
    {
        gdk_draw_line(window, style->dark_gc[state_type], x + i, y_2 - i - 1, x + i, y_2);
        gdk_draw_line(window, style->dark_gc[state_type], x + i, y_1, x + i, y_2 - i - 1);
    }

    x += thickness_dark;
    for(i = 0; i < thickness_light; i++)
    {
        gdk_draw_line(window, style->light_gc[state_type], x + i, y_1, x + i, y_1 + thickness_light - i);
        gdk_draw_line(window, style->light_gc[state_type], x + i, y_1 + thickness_light - i, x + i, y_2);
    }
    if (area)
    {
        gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
        gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
    }
}

static void draw_shadow(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    gint xt, yt;

    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    /* Spin buttons are a special case */
    if (widget && GTK_IS_SPIN_BUTTON (widget))
    {
        /* GtkEntry doesn't provide the text area as frame size */
        if (DETAIL("entry"))
        {
            gint line_width = 0;
            gboolean interior_focus;
            gdk_drawable_get_size (GTK_ENTRY(widget)->text_area, &width, NULL);
            if (!gtk_widget_has_focus(widget))
                gtk_widget_style_get(widget, "focus-line-width", &line_width, "interior-focus", &interior_focus, NULL);
            if (interior_focus)
                line_width = 0;
            width += (style->xthickness + line_width) * 2;
        }
    }

    if (shadow_type == GTK_SHADOW_NONE)
        return;

    if ((width == -1) && (height == -1))
        gdk_drawable_get_size(window, &width, &height);
    else if (width == -1)
        gdk_drawable_get_size(window, &width, NULL);
    else if (height == -1)
        gdk_drawable_get_size(window, NULL, &height);

    if (area)
    {
        gdk_gc_set_clip_rectangle(style->light_gc[state_type], area);
        gdk_gc_set_clip_rectangle(style->dark_gc[state_type], area);
        gdk_gc_set_clip_rectangle(style->mid_gc[state_type], area);
        gdk_gc_set_clip_rectangle(style->black_gc, area);
        gdk_gc_set_clip_rectangle(style->bg_gc[state_type], area);
    }

    xt = MIN(style->xthickness, width - 1);
    yt = MIN(style->ythickness, height - 1);

    switch (shadow_type)
    {
        case GTK_SHADOW_NONE:
            break;
        case GTK_SHADOW_ETCHED_IN:
            if ((xt > 1) && (yt > 1))
            {
                gdk_draw_line(window, style->light_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                gdk_draw_line(window, style->light_gc[state_type], x + width - 1, y, x + width - 1, y + height - 1);

                gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width - 2, y);
                gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height - 2);

                gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + width - 2, y + 1);
                gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + 1, y + height - 2);

                gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 2, x + width - 2, y + height - 2);
                gdk_draw_line(window, style->dark_gc[state_type], x + width - 2, y + 1, x + width - 2, y + height - 2);
            }
            else if ((xt > 0) && (yt > 0))
            {
                gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y, x + width - 1, y + height - 1);

                gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width - 1, y);
                gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height - 1);
            }
            break;
        case GTK_SHADOW_ETCHED_OUT:
            if ((xt > 1) && (yt > 1))
            {
                gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y, x + width - 1, y + height - 1);

                gdk_draw_line(window, style->light_gc[state_type], x, y, x + width - 2, y);
                gdk_draw_line(window, style->light_gc[state_type], x, y, x, y + height - 2);

                gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + 1, x + width - 2, y + 1);
                gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + 1, x + 1, y + height - 2);

                gdk_draw_line(window, style->light_gc[state_type], x + 1, y + height - 2, x + width - 2, y + height - 2);
                gdk_draw_line(window, style->light_gc[state_type], x + width - 2, y + 1, x + width - 2, y + height - 2);
            }
            else if ((xt > 0) && (yt > 0))
            {
                gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + width - 2, y + 1);
                gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + 1, y + height - 2);

                gdk_draw_line(window, style->light_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                gdk_draw_line(window, style->light_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);
            }
            break;
        case GTK_SHADOW_IN:
            if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
            {
                if (DETAIL("trough"))
                {
                    gdk_draw_line(window, style->bg_gc[GTK_STATE_NORMAL], x, y, x + width - 1, y);
                    gdk_draw_line(window, style->bg_gc[GTK_STATE_NORMAL], x, y, x, y + height - 1);

                    gdk_draw_line(window, style->bg_gc[GTK_STATE_NORMAL], x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, style->bg_gc[GTK_STATE_NORMAL], x + width - 1, y + 1, x + width - 1, y + height - 1);

                    gdk_draw_point(window, style->mid_gc[GTK_STATE_NORMAL], x, y);
                    gdk_draw_point(window, style->mid_gc[GTK_STATE_NORMAL], x + width - 1, y);
                    gdk_draw_point(window, style->mid_gc[GTK_STATE_NORMAL], x, y + height - 1);
                    gdk_draw_point(window, style->mid_gc[GTK_STATE_NORMAL], x + width - 1, y + height - 1);
                }
                else if ((xt > 1) && (yt > 1))
                {
                    gdk_draw_line(window, style->dark_gc[state_type], x + 1, y, x + width - 2, y);
                    gdk_draw_line(window, style->dark_gc[state_type], x, y + 1, x, y + height - 2);

                    gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 1, x + width - 2, y + height - 1);
                    gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 2);

                    gdk_draw_line(window, style->mid_gc[state_type], x + 1, y + 1, x + width - 2, y + 1);
                    gdk_draw_line(window, style->mid_gc[state_type], x + 1, y + 1, x + 1, y + height - 2);

                    gdk_draw_line(window, style->light_gc[state_type], x + 2, y + height - 2, x + width - 2, y + height - 2);
                    gdk_draw_line(window, style->light_gc[state_type], x + width - 2, y + 2, x + width - 2, y + height - 2);

                    gdk_draw_point(window, style->mid_gc[state_type], x, y);
                    gdk_draw_point(window, style->mid_gc[state_type], x + width - 1, y);
                    gdk_draw_point(window, style->mid_gc[state_type], x, y + height - 1);
                    gdk_draw_point(window, style->mid_gc[state_type], x + width - 1, y + height - 1);
                }
                else if ((xt > 0) && (yt > 0))
                {
                    gdk_draw_line(window, style->dark_gc[state_type], x + 1, y, x + width - 1, y);
                    gdk_draw_line(window, style->dark_gc[state_type], x, y + 1, x, y + height - 1);

                    gdk_draw_line(window, style->light_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, style->light_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);

                    gdk_draw_point(window, style->mid_gc[state_type], x, y);
                    gdk_draw_point(window, style->mid_gc[state_type], x + width - 1, y);
                    gdk_draw_point(window, style->mid_gc[state_type], x, y + height - 1);
                    gdk_draw_point(window, style->mid_gc[state_type], x + width - 1, y + height - 1);
                }
            }
            else
            {
                if (DETAIL("trough"))
                {
                    gdk_draw_line(window, style->dark_gc[GTK_STATE_ACTIVE], x, y, x + width - 1, y);
                    gdk_draw_line(window, style->dark_gc[GTK_STATE_ACTIVE], x, y, x, y + height - 1);

                    gdk_draw_line(window, style->dark_gc[GTK_STATE_ACTIVE], x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, style->dark_gc[GTK_STATE_ACTIVE], x + width - 1, y + 1, x + width - 1, y + height - 1);
                }
                else if ((xt > 1) && (yt > 1))
                {
                    gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width - 1, y);
                    gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height - 1);

                    gdk_draw_line(window, style->light_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, style->light_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);

                    gdk_draw_line(window, style->black_gc, x + 1, y + 1, x + width - 2, y + 1);
                    gdk_draw_line(window, style->black_gc, x + 1, y + 1, x + 1, y + height - 2);

                    gdk_draw_line(window, style->dark_gc[state_type], x + 2, y + height - 2, x + width - 2, y + height - 2);
                    gdk_draw_line(window, style->dark_gc[state_type], x + width - 2, y + 2, x + width - 2, y + height - 2);
                }
                else if ((xt > 0) && (yt > 0))
                {
                    gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width - 1, y);
                    gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height - 1);

                    gdk_draw_line(window, style->light_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, style->light_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);
                }
            }
            break;
        case GTK_SHADOW_OUT:
            if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
            {
                if ((DETAIL("spinbutton_up") || DETAIL("spinbutton_down")) && (state_type != GTK_STATE_PRELIGHT))
                {
                    /* Do nothing */
                }
                else if (DETAIL("bar"))
                {
                    gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width - 1, y);
                    gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height - 1);

                    gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);

                    gdk_draw_point(window, style->mid_gc[GTK_STATE_NORMAL], x, y);
                    gdk_draw_point(window, style->mid_gc[GTK_STATE_NORMAL], x + width - 1, y);
                    gdk_draw_point(window, style->mid_gc[GTK_STATE_NORMAL], x, y + height - 1);
                    gdk_draw_point(window, style->mid_gc[GTK_STATE_NORMAL], x + width - 1, y + height - 1);
                }
                else if (DETAIL("menu"))
                {
                    if ((xt > 0) && (yt > 0))
                    {
                        gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width - 1, y);
                        gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height - 1);

                        gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                        gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);
                    }
                }
                else if (DETAIL("menubar"))
                {
                    if ((xt > 1) && (yt > 1))
                    {
                        gdk_draw_line(window, style->mid_gc[state_type], x , y + height - 2, x + width - 1, y + height - 2);
                        gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                    }
                }
                else if (DETAIL ("dockitem_bin") || DETAIL ("dockitem") || DETAIL ("toolbar"))
                {
                    if ((xt > 1) && (yt > 1))
                    {
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x + width - 1, y);
                        gdk_draw_line(window, style->mid_gc[state_type], x , y + height - 2, x + width - 1, y + height - 2);
                        gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x + width - 1, y);
                        gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                    }
                }
                else if (DETAIL("vscrollbar") || DETAIL("hscrollbar") || DETAIL("bar") || DETAIL("slider") || DETAIL("vscale") || DETAIL("hscale"))
                {
                    if ((xt > 1) && (yt > 1))
                    {
                        gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width - 1, y);
                        gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height - 1);

                        gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                        gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);

                        gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + width - 3, y + 1);
                        gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + 1, y + height - 3);

                        gdk_draw_line(window, style->mid_gc[state_type], x + 1, y + height - 2, x + width - 2, y + height - 2);
                        gdk_draw_line(window, style->mid_gc[state_type], x + width - 2, y + 1, x + width - 2, y + height - 2);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x + width - 1, y);
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x, y + height - 1);

                        gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                        gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);
                    }
                }
                else if (DETAIL("menubar") || DETAIL("frame") || DETAIL("dockitem") || DETAIL("dockitem_bin") || DETAIL("menu") || DETAIL("toolbar"))
                {
                    if ((xt > 1) && (yt > 1))
                    {
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x + width - 2, y);
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x, y + height - 2);

                        gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                        gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y, x + width - 1, y + height - 1);

                        gdk_draw_line(window, style->bg_gc[state_type], x + 1, y + 1, x + width - 2, y + 1);
                        gdk_draw_line(window, style->bg_gc[state_type], x + 1, y + 1, x + 1, y + height - 2);

                        gdk_draw_line(window, style->bg_gc[state_type], x + 2, y + height - 2, x + width - 2, y + height - 2);
                        gdk_draw_line(window, style->bg_gc[state_type], x + width - 2, y + 2, x + width - 2, y + height - 2);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x + width - 1, y);
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x, y + height - 1);

                        gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                        gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);
                    }
                }
                else
                {
                    if ((xt > 1) && (yt > 1))
                    {
                        gdk_draw_line(window, style->dark_gc[state_type], x + 1, y, x + width - 2, y);
                        gdk_draw_line(window, style->dark_gc[state_type], x, y + 1, x, y + height - 2);

                        gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 1, x + width - 2, y + height - 1);
                        gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 2);

                        gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + width - 3, y + 1);
                        gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + 1, y + height - 3);

                        gdk_draw_line(window, style->mid_gc[state_type], x + 1, y + height - 2, x + width - 2, y + height - 2);
                        gdk_draw_line(window, style->mid_gc[state_type], x + width - 2, y + 1, x + width - 2, y + height - 2);

                        gdk_draw_point(window, style->mid_gc[state_type], x, y);
                        gdk_draw_point(window, style->mid_gc[state_type], x + width - 1, y);
                        gdk_draw_point(window, style->mid_gc[state_type], x, y + height - 1);
                        gdk_draw_point(window, style->mid_gc[state_type], x + width - 1, y + height - 1);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_draw_line(window, style->light_gc[state_type], x + 1, y, x + width - 1, y);
                        gdk_draw_line(window, style->light_gc[state_type], x, y + 1, x, y + height - 1);

                        gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                        gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);

                        gdk_draw_point(window, style->mid_gc[state_type], x, y);
                        gdk_draw_point(window, style->mid_gc[state_type], x + width - 1, y);
                        gdk_draw_point(window, style->mid_gc[state_type], x, y + height - 1);
                        gdk_draw_point(window, style->mid_gc[state_type], x + width - 1, y + height - 1);
                    }
                }
            }
            else
            {
                if ((DETAIL("spinbutton_up") || DETAIL("spinbutton_down")) && (state_type != GTK_STATE_PRELIGHT))
                {
                    /* Do nothing */
                }
                else if (DETAIL("menubar"))
                {
                    if ((xt > 1) && (yt > 1))
                    {
                        gdk_draw_line(window, style->mid_gc[state_type], x , y + height - 2, x + width - 1, y + height - 2);
                        gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                    }
                }
                else if (DETAIL ("dockitem_bin") || DETAIL ("dockitem") || DETAIL ("toolbar"))
                {
                    if ((xt > 1) && (yt > 1))
                    {
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x + width - 1, y);
                        gdk_draw_line(window, style->mid_gc[state_type], x , y + height - 2, x + width - 1, y + height - 2);
                        gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                    }
                    else if ((xt > 0) && (yt > 0))
                    {
                        gdk_draw_line(window, style->light_gc[state_type], x, y, x + width - 1, y);
                        gdk_draw_line(window, style->dark_gc[state_type], x, y + height - 1, x + width - 1, y + height - 1);
                    }
                }
                else if ((xt > 1) && (yt > 1))
                {
                    gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width - 1, y);
                    gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height - 1);

                    gdk_draw_line(window, style->black_gc, x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, style->black_gc, x + width - 1, y + 1, x + width - 1, y + height - 1);

                    gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + width - 2, y + 1);
                    gdk_draw_line(window, style->light_gc[state_type], x + 1, y + 1, x + 1, y + height - 2);

                    gdk_draw_line(window, style->dark_gc[state_type], x + 2, y + height - 2, x + width - 2, y + height - 2);
                    gdk_draw_line(window, style->dark_gc[state_type], x + width - 2, y + 2, x + width - 2, y + height - 2);
                }
                else if ((xt > 0) && (yt > 0))
                {
                    gdk_draw_line(window, style->light_gc[state_type], x, y, x + width - 1, y);
                    gdk_draw_line(window, style->light_gc[state_type], x, y, x, y + height - 1);

                    gdk_draw_line(window, style->dark_gc[state_type], x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, style->dark_gc[state_type], x + width - 1, y + 1, x + width - 1, y + height - 1);
                }
            }
            break;
    }
    if (area)
    {
        gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
        gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
        gdk_gc_set_clip_rectangle(style->mid_gc[state_type], NULL);
        gdk_gc_set_clip_rectangle(style->black_gc, NULL);
        gdk_gc_set_clip_rectangle(style->bg_gc[state_type], NULL);
    }
}

static void draw_box(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    if ((width == -1) && (height == -1))
        gdk_drawable_get_size(window, &width, &height);
    else if (width == -1)
        gdk_drawable_get_size(window, &width, NULL);
    else if (height == -1)
        gdk_drawable_get_size(window, NULL, &height);

    if (!style->bg_pixmap[state_type])
    {
        xfce_fill_background(style, window, state_type, area, widget, detail, x, y, width, height);
    }
    else
    {
        gtk_style_apply_default_background(style, window, 1, state_type, area, x, y, width, height);
    }
    draw_shadow(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
}

static GdkBitmap *get_part_bmap (GdkDrawable *drawable, Part part)
{
#if GTK_CHECK_VERSION(2, 2, 0)
    GdkScreen *screen = gdk_drawable_get_screen (drawable);
    GdkBitmap *bitmap;
    GList *tmp_list;

    tmp_list = parts[part].bmap_list;
    while (tmp_list)
    {
        bitmap = tmp_list->data;
        if (gdk_drawable_get_screen (bitmap) == screen)
        {
            return bitmap;
        }
        tmp_list = tmp_list->next;
    }

    bitmap = gdk_bitmap_create_from_data (drawable, (gchar *)parts[part].bits, PART_SIZE, PART_SIZE);
    parts[part].bmap_list = g_list_prepend (parts[part].bmap_list, bitmap);

    return bitmap;
#else
    if (!parts[part].bmap)
    {
        parts[part].bmap = gdk_bitmap_create_from_data(drawable, parts[part].bits, PART_SIZE, PART_SIZE);
    }
    return parts[part].bmap;
#endif
}

static void draw_part(GdkDrawable * drawable, GdkGC * gc, GdkRectangle * area, gint x, gint y, Part part)
{
    if (area)
    {
        gdk_gc_set_clip_rectangle(gc, area);
    }

    gdk_gc_set_ts_origin(gc, x, y);
    gdk_gc_set_stipple(gc, get_part_bmap (drawable, part));
    gdk_gc_set_fill(gc, GDK_STIPPLED);

    gdk_draw_rectangle(drawable, gc, TRUE, x, y, PART_SIZE, PART_SIZE);

    gdk_gc_set_fill(gc, GDK_SOLID);

    if (area)
    {
        gdk_gc_set_clip_rectangle(gc, NULL);
    }
}

static void draw_check(GtkStyle * style, GdkWindow * window, GtkStateType state, GtkShadowType shadow, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    x -= (1 + PART_SIZE - width) / 2;
    y -= (1 + PART_SIZE - height) / 2;

    if (DETAIL("check"))    /* Menu item */
    {
#if 0
        draw_part(window, style->bg_gc[state], area, x, y, CHECK_BASE);
#endif
        draw_part(window, style->dark_gc[state], area, x, y, CHECK_LIGHT);
        draw_part(window, style->dark_gc[state], area, x, y, CHECK_DARK);

        if (shadow == GTK_SHADOW_IN)
        {
            draw_part(window, style->fg_gc[state], area, x, y, CHECK_CROSS);
        }
    }
    else
    {
        draw_part(window, style->base_gc[state], area, x, y, CHECK_BASE);
        draw_part(window, style->dark_gc[state], area, x, y, CHECK_LIGHT);
        draw_part(window, style->dark_gc[state], area, x, y, CHECK_DARK);

        if (shadow == GTK_SHADOW_IN)
        {
            draw_part(window, style->text_gc[state], area, x, y, CHECK_CROSS);
        }
    }
}

static void draw_option(GtkStyle * style, GdkWindow * window, GtkStateType state, GtkShadowType shadow, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    x -= (1 + PART_SIZE - width) / 2;
    y -= (1 + PART_SIZE - height) / 2;

    if (DETAIL("option"))   /* Menu item */
    {
#if 0
        draw_part(window, style->bg_gc[state], area, x, y, RADIO_BASE);
#endif
        draw_part(window, style->dark_gc[state], area, x, y, RADIO_LIGHT);
        draw_part(window, style->dark_gc[state], area, x, y, RADIO_DARK);

        if (shadow == GTK_SHADOW_IN)
        {
            draw_part(window, style->fg_gc[state], area, x, y, RADIO_TEXT);
        }
    }
    else
    {
        draw_part(window, style->base_gc[state], area, x, y, RADIO_BASE);
        draw_part(window, style->dark_gc[state], area, x, y, RADIO_LIGHT);
        draw_part(window, style->dark_gc[state], area, x, y, RADIO_DARK);

        if (shadow == GTK_SHADOW_IN)
        {
            draw_part(window, style->text_gc[state], area, x, y, RADIO_TEXT);
        }
    }
}

static void draw_shadow_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width)
{
    GdkRectangle rect;

    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    if (shadow_type == GTK_SHADOW_NONE)
        return;

    draw_shadow(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);

    switch (gap_side)
    {
        case GTK_POS_TOP:
            rect.x = x + gap_x;
            rect.y = y;
            rect.width = gap_width;
            rect.height = 2;
            break;
        case GTK_POS_BOTTOM:
            rect.x = x + gap_x;
            rect.y = y + height - 2;
            rect.width = gap_width;
            rect.height = 2;
            break;
        case GTK_POS_LEFT:
            rect.x = x;
            rect.y = y + gap_x;
            rect.width = 2;
            rect.height = gap_width;
            break;
        default: /* GTK_POS_RIGHT */
            rect.x = x + width - 2;
            rect.y = y + gap_x;
            rect.width = 2;
            rect.height = gap_width;
            break;
    }

    gtk_style_apply_default_background(style, window, 1, state_type, area, rect.x, rect.y, rect.width, rect.height);
}

static void draw_box_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width)
{
    GdkGC *gc1 = NULL;
    GdkGC *gc2 = NULL;
    GdkGC *gc3 = NULL;
    GdkGC *gc4 = NULL;

    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x, y, width, height);

    if (width == -1 && height == -1)
        gdk_drawable_get_size(window, &width, &height);
    else if (width == -1)
        gdk_drawable_get_size(window, &width, NULL);
    else if (height == -1)
        gdk_drawable_get_size(window, NULL, &height);

    if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
    {
        gc1 = style->dark_gc[state_type];
        gc2 = style->bg_gc[state_type];
        gc3 = style->bg_gc[state_type];
        gc4 = style->dark_gc[state_type];
    }
    else
    {
        gc1 = style->dark_gc[state_type];
        gc2 = style->light_gc[state_type];
        gc3 = style->dark_gc[state_type];
        gc4 = style->black_gc;
    }

    if (area)
    {
        gdk_gc_set_clip_rectangle(gc1, area);
        gdk_gc_set_clip_rectangle(gc2, area);
        gdk_gc_set_clip_rectangle(gc3, area);
        gdk_gc_set_clip_rectangle(gc4, area);
    }

    switch (shadow_type)
    {
        case GTK_SHADOW_NONE:
        case GTK_SHADOW_IN:
        case GTK_SHADOW_OUT:
        case GTK_SHADOW_ETCHED_IN:
        case GTK_SHADOW_ETCHED_OUT:
            switch (gap_side)
            {
                case GTK_POS_TOP:
                    gdk_draw_line(window, gc1, x, y, x, y + height - 1);
                    gdk_draw_line(window, gc2, x + 1, y + 1, x + 1, y + height - 2);

                    gdk_draw_line(window, gc3, x + 2, y + height - 2, x + width - 2, y + height - 2);
                    gdk_draw_line(window, gc3, x + width - 2, y, x + width - 2, y + height - 2);

                    gdk_draw_line(window, gc4, x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, gc4, x + width - 1, y, x + width - 1, y + height - 1);
                    if (gap_x > 0)
                    {
                        gdk_draw_line(window, gc1, x, y, x + gap_x - 1, y);
                        gdk_draw_line(window, gc2, x + 1, y + 1, x + gap_x - 1, y + 1);
                        gdk_draw_line(window, gc2, x + gap_x, y, x + gap_x, y);
                    }
                    if ((width - (gap_x + gap_width)) > 0)
                    {
                        gdk_draw_line(window, gc1, x + gap_x + gap_width, y, x + width - 2, y);
                        gdk_draw_line(window, gc2, x + gap_x + gap_width, y + 1, x + width - 2, y + 1);
                        gdk_draw_line(window, gc2, x + gap_x + gap_width - 1, y, x + gap_x + gap_width - 1, y);
                    }
                    break;
                case GTK_POS_BOTTOM:
                    gdk_draw_line(window, gc1, x, y, x + width - 1, y);
                    gdk_draw_line(window, gc1, x, y, x, y + height - 1);

                    gdk_draw_line(window, gc2, x + 1, y + 1, x + width - 2, y + 1);
                    gdk_draw_line(window, gc2, x + 1, y + 1, x + 1, y + height - 1);

                    gdk_draw_line(window, gc3, x + width - 2, y + 1, x + width - 2, y + height - 1);
                    gdk_draw_line(window, gc4, x + width - 1, y, x + width - 1, y + height - 1);
                    if (gap_x > 0)
                    {
                        gdk_draw_line(window, gc4, x, y + height - 1, x + gap_x - 1, y + height - 1);
                        gdk_draw_line(window, gc3, x + 1, y + height - 2, x + gap_x - 1, y + height - 2);
                        gdk_draw_line(window, gc3, x + gap_x, y + height - 1, x + gap_x, y + height - 1);
                    }
                    if ((width - (gap_x + gap_width)) > 0)
                    {
                        gdk_draw_line(window, gc4, x + gap_x + gap_width, y + height - 1, x + width - 2, y + height - 1);
                        gdk_draw_line(window, gc3, x + gap_x + gap_width, y + height - 2, x + width - 2, y + height - 2);
                        gdk_draw_line(window, gc3, x + gap_x + gap_width - 1, y + height - 1, x + gap_x + gap_width - 1, y + height - 1);
                    }
                    break;
                case GTK_POS_LEFT:
                    gdk_draw_line(window, gc1, x, y, x + width - 1, y);
                    gdk_draw_line(window, gc2, x, y + 1, x + width - 2, y + 1);

                    gdk_draw_line(window, gc3, x, y + height - 2, x + width - 2, y + height - 2);
                    gdk_draw_line(window, gc3, x + width - 2, y + 1, x + width - 2, y + height - 2);

                    gdk_draw_line(window, gc4, x + 1, y + height - 1, x + width - 1, y + height - 1);
                    gdk_draw_line(window, gc4, x + width - 1, y, x + width - 1, y + height - 1);
                    if (gap_x > 0)
                    {
                        gdk_draw_line(window, gc1, x, y, x, y + gap_x - 1);
                        gdk_draw_line(window, gc2, x + 1, y + 1, x + 1, y + gap_x - 1);
                        gdk_draw_line(window, gc2, x, y + gap_x, x, y + gap_x);
                    }
                    if ((width - (gap_x + gap_width)) > 0)
                    {
                        gdk_draw_line(window, gc1, x, y + gap_x + gap_width, x, y + height - 2);
                        gdk_draw_line(window, gc2, x + 1, y + gap_x + gap_width, x + 1, y + height - 2);
                        gdk_draw_line(window, gc2, x, y + gap_x + gap_width - 1, x, y + gap_x + gap_width - 1);
                    }
                    break;
                case GTK_POS_RIGHT:
                    gdk_draw_line(window, gc1, x, y, x + width - 1, y);
                    gdk_draw_line(window, gc1, x, y, x, y + height - 1);

                    gdk_draw_line(window, gc2, x + 1, y + 1, x + width - 1, y + 1);
                    gdk_draw_line(window, gc2, x + 1, y + 1, x + 1, y + height - 2);

                    gdk_draw_line(window, gc3, x + 1, y + height - 2, x + width - 1, y + height - 2);
                    gdk_draw_line(window, gc4, x + 1, y + height - 1, x + width - 1, y + height - 1);
                    if (gap_x > 0)
                    {
                        gdk_draw_line(window, gc4, x + width - 1, y, x + width - 1, y + gap_x - 1);
                        gdk_draw_line(window, gc3, x + width - 2, y + 1, x + width - 2, y + gap_x - 1);
                        gdk_draw_line(window, gc3, x + width - 1, y + gap_x, x + width - 1, y + gap_x);
                    }
                    if ((width - (gap_x + gap_width)) > 0)
                    {
                        gdk_draw_line(window, gc4, x + width - 1, y + gap_x + gap_width, x + width - 1, y + height - 2);
                        gdk_draw_line(window, gc3, x + width - 2, y + gap_x + gap_width, x + width - 2, y + height - 2);
                        gdk_draw_line(window, gc3, x + width - 1, y + gap_x + gap_width - 1, x + width - 1, y + gap_x + gap_width - 1);
                    }
                    break;
            }
    }

    if (area)
    {
        gdk_gc_set_clip_rectangle(gc1, NULL);
        gdk_gc_set_clip_rectangle(gc2, NULL);
        gdk_gc_set_clip_rectangle(gc3, NULL);
        gdk_gc_set_clip_rectangle(gc4, NULL);
    }
}

static void draw_extension(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side)
{
    GdkGC *gc1 = NULL;
    GdkGC *gc2 = NULL;
    GdkGC *gc3 = NULL;
    GdkGC *gc4 = NULL;

    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), GTK_STATE_NORMAL, area, x, y, width, height);

    if (width == -1 && height == -1)
        gdk_drawable_get_size(window, &width, &height);
    else if (width == -1)
        gdk_drawable_get_size(window, &width, NULL);
    else if (height == -1)
        gdk_drawable_get_size(window, NULL, &height);

    if (XFCE_RC_STYLE(style->rc_style)->smooth_edge)
    {
        gc1 = style->dark_gc[state_type];
        gc2 = style->bg_gc[state_type];
        gc3 = style->bg_gc[state_type];
        gc4 = style->dark_gc[state_type];
    }
    else
    {
        gc1 = style->dark_gc[state_type];
        gc2 = style->light_gc[state_type];
        gc3 = style->dark_gc[state_type];
        gc4 = style->black_gc;
    }

    if (area)
    {
        gdk_gc_set_clip_rectangle(gc1, area);
        gdk_gc_set_clip_rectangle(gc2, area);
        gdk_gc_set_clip_rectangle(gc3, area);
        gdk_gc_set_clip_rectangle(gc4, area);
    }

    switch (shadow_type)
    {
        case GTK_SHADOW_NONE:
        case GTK_SHADOW_IN:
        case GTK_SHADOW_OUT:
        case GTK_SHADOW_ETCHED_IN:
        case GTK_SHADOW_ETCHED_OUT:
            switch (gap_side)
            {
                case GTK_POS_TOP:
                    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x + 1, y, width - 2, height - 1);
                    gdk_draw_line(window, gc1, x, y, x, y + height - 2);
                    gdk_draw_line(window, gc2, x + 1, y + 1, x + 1, y + height - 2);

                    gdk_draw_line(window, gc3, x + 2, y + height - 2, x + width - 2, y + height - 2);
                    gdk_draw_line(window, gc3, x + width - 2, y, x + width - 2, y + height - 2);
                    gdk_draw_line(window, gc4, x + 1, y + height - 1, x + width - 2, y + height - 1);
                    gdk_draw_line(window, gc4, x + width - 1, y, x + width - 1, y + height - 2);
                    break;
                case GTK_POS_BOTTOM:
                    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x + style->xthickness, y + style->ythickness, width - (2 * style->xthickness), height - (style->ythickness));
                    gdk_draw_line(window, gc1, x + 1, y, x + width - 2, y);
                    gdk_draw_line(window, gc1, x, y + 1, x, y + height - 1);
                    gdk_draw_line(window, gc2, x + 1, y + 1, x + width - 2, y + 1);
                    gdk_draw_line(window, gc2, x + 1, y + 1, x + 1, y + height - 1);

                    gdk_draw_line(window, gc3, x + width - 2, y + 2, x + width - 2, y + height - 1);
                    gdk_draw_line(window, gc4, x + width - 1, y + 1, x + width - 1, y + height - 1);
                    break;
                case GTK_POS_LEFT:
                    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x, y + style->ythickness, width - (style->xthickness), height - (2 * style->ythickness));
                    gdk_draw_line(window, gc1, x, y, x + width - 2, y);
                    gdk_draw_line(window, gc2, x + 1, y + 1, x + width - 2, y + 1);

                    gdk_draw_line(window, gc3, x, y + height - 2, x + width - 2, y + height - 2);
                    gdk_draw_line(window, gc3, x + width - 2, y + 2, x + width - 2, y + height - 2);
                    gdk_draw_line(window, gc4, x, y + height - 1, x + width - 2, y + height - 1);
                    gdk_draw_line(window, gc4, x + width - 1, y + 1, x + width - 1, y + height - 2);
                    break;
                case GTK_POS_RIGHT:
                    gtk_style_apply_default_background(style, window, widget && !GTK_WIDGET_NO_WINDOW(widget), state_type, area, x + style->xthickness, y + style->ythickness, width - (style->xthickness), height - (2 * style->ythickness));
                    gdk_draw_line(window, gc1, x + 1, y, x + width - 1, y);
                    gdk_draw_line(window, gc1, x, y + 1, x, y + height - 2);
                    gdk_draw_line(window, gc2, x + 1, y + 1, x + width - 1, y + 1);
                    gdk_draw_line(window, gc2, x + 1, y + 1, x + 1, y + height - 2);

                    gdk_draw_line(window, gc3, x + 2, y + height - 2, x + width - 1, y + height - 2);
                    gdk_draw_line(window, gc4, x + 1, y + height - 1, x + width - 1, y + height - 1);
                    break;
            }
    }

    if (area)
    {
        gdk_gc_set_clip_rectangle(gc1, NULL);
        gdk_gc_set_clip_rectangle(gc2, NULL);
        gdk_gc_set_clip_rectangle(gc3, NULL);
        gdk_gc_set_clip_rectangle(gc4, NULL);
    }
}

static void draw_focus(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height)
{
    /* Spin buttons are a special case */
    if (widget && GTK_IS_SPIN_BUTTON (widget))
    {
        /* GtkEntry doesn't provide the text area as focus size */
        if (DETAIL("entry"))
        {
            gint line_width = 0;
            gboolean interior_focus;
            gdk_drawable_get_size (GTK_ENTRY(widget)->text_area, &width, NULL);
            gtk_widget_style_get (widget, "focus-line-width", &line_width, "interior-focus", &interior_focus, NULL);
            if (interior_focus)
                line_width = 0;
            width += (style->xthickness + line_width) * 2;
        }
    }

    parent_class->draw_focus(style, window, state_type, area, widget, detail, x, y, width, height);
}

static void draw_slider(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    if ((width == -1) && (height == -1))
        gdk_drawable_get_size(window, &width, &height);
    else if (width == -1)
        gdk_drawable_get_size(window, &width, NULL);
    else if (height == -1)
        gdk_drawable_get_size(window, NULL, &height);

    orientation = GTK_ORIENTATION_HORIZONTAL;
    if (height > width)
        orientation = GTK_ORIENTATION_VERTICAL;

    draw_box(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
    xfce_draw_grips(style, window, state_type, area, widget, x, y, width, height, orientation);
}

static void draw_handle(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, const gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    if ((width == -1) && (height == -1))
        gdk_drawable_get_size(window, &width, &height);
    else if (width == -1)
        gdk_drawable_get_size(window, &width, NULL);
    else if (height == -1)
        gdk_drawable_get_size(window, NULL, &height);

    orientation = GTK_ORIENTATION_HORIZONTAL;
    if (height > width)
        orientation = GTK_ORIENTATION_VERTICAL;

#if 0
    draw_box(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
#endif
    xfce_draw_grips(style, window, state_type, area, widget, x, y, width, height, orientation);
}

GType xfce_type_style = 0;

void xfce_style_register_type(GTypeModule * module)
{
    static const GTypeInfo object_info = {
        sizeof(XfceStyleClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) xfce_style_class_init,
        NULL,                   /* class_finalize */
        NULL,                   /* class_data */
        sizeof(XfceStyle),
        0,                      /* n_preallocs */
        (GInstanceInitFunc) xfce_style_init,
    };

    xfce_type_style = g_type_module_register_type(module, GTK_TYPE_STYLE, "XfceStyle", &object_info, 0);
}

static void xfce_style_init(XfceStyle * style)
{
}

static void xfce_style_class_init(XfceStyleClass * klass)
{
    GtkStyleClass *style_class = GTK_STYLE_CLASS(klass);

    parent_class = g_type_class_peek_parent(klass);
    style_class->draw_hline = draw_hline;
    style_class->draw_vline = draw_vline;
    style_class->draw_shadow = draw_shadow;
    style_class->draw_box = draw_box;
    style_class->draw_check = draw_check;
    style_class->draw_option = draw_option;
    style_class->draw_shadow_gap = draw_shadow_gap;
    style_class->draw_box_gap = draw_box_gap;
    style_class->draw_extension = draw_extension;
    style_class->draw_focus = draw_focus;
    style_class->draw_slider = draw_slider;
    style_class->draw_handle = draw_handle;
}
