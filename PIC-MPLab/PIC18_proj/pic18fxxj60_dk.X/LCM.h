/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */
#ifndef __LCM_H__
#define __LCM_H__



spVOIDt LCM_Init(spVOIDt);
spVOIDt LCM_Send_Byte(spUINT8t nn);
spVOIDt LCM_Char(spUINT8t ch);
spVOIDt LCM_Cmd(spUINT8t cmd);
spVOIDt LCM_String(spUINT8t* pString);

#define cmdLCM_CR       {LCM_Cmd(0x02);}



#endif /* __LCM_H__ */