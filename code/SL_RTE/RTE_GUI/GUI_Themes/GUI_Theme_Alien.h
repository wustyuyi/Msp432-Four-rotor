/**
 * @file theme_alien.h
 *
 */

#ifndef GUI_THEME_ALIEN_H
#define GUI_THEME_ALIEN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "RTE_Include.h"

#if GUI_USE_THEME_ALIEN

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the alien theme
 * @param hue [0..360] hue value from HSV color space to define the theme's base color
 * @param font pointer to a font (NULL to use the default)
 * @return pointer to the initialized theme
 */
theme_t * theme_alien_init(uint16_t hue, font_t *font);
/**
 * Get a pointer to the theme
 * @return pointer to the theme
 */
theme_t * theme_get_alien(void);

/**********************
 *      MACROS
 **********************/

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*GUI_THEME_ALIEN_H*/
