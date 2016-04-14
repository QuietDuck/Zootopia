#pragma once

#ifndef __Z_COMPOSE_H__
#define __Z_COMPOSE_H__

// THERE IS A IDE BUG. IF WE USE THIS, IDE CAN'T NOT FIND CTOR.
/*
#ifdef __cplusplus
#	define ZOOTOPIA_BEGIN				namespace zootopia {
#	define ZOOTOPIA_END                 }
#	define ZOOTOPIA_USE					using namespace zootopia;
#else
#	define ZOOTOPIA_BEGIN
#	define ZOOTOPIA_END
#	define ZOOTOPIA_USE
#endif
*/

#ifdef __cplusplus
#	define null					nullptr
#else
#	define null					((void*)0)
#endif

#define ZBUFFER_OFFSET(i) ((char *)null + (i))

/**
avoid compile error cased by not used param
**/
#define zunused(param)			(void)(param)

#define ZFLAG_SET(f,v)			((f) |= (v))

#define ZFLAG_UNSET(f,v)		((f) &= ~(v))

#define ZFLAG_IS_SET(f,v)		(((f) & (v)) == (v))

#define ZFLAG_HAS_ANY_OF(f,v)	(((f) & (v)) != 0)

#define ZFLAG_GET(f,mask)		((f) & (mask))

#define EQ ==

#define NEQ !=

#define AND &&

#define OR ||


// jump to 'label' if 'exp' is true
// DEPRECATE.
//#define ZJIF(exp, label) do{ if(exp) goto label; }while(0)

#define ZDELETE(p) delete p

#define ZDELETEZ(p) do{ delete p; p = null;} while (0)

#define ZDELETE_SAFE(p) do{ if(p) delete p; } while(0)

#define ZDELETEZ_SAFE(p) do{ ZDELETE_SAFE(p); p = null; } while(0)

#define ZDELETE_ARRAY(p) do{ delete[] p;} while(0)

#define ZDELETEZ_ARRAY(p) do{ delete[] p; p = null;} while(0)

#define ZDELETE_ARRAY_SAFE(p) do{ if(p) delete [] p; } while(0)

#define ZDELETEZ_ARRAY_SAFE(p) do{ ZDELETE_ARRAY_SAFE(p); p = null; } while(0)


#define __zpd_macro_concat_a_b(a, b)                a ## b
#define __zpd_macro_make_string(s)                  # s


#define ZCONCAT(a, b)							__zpd_macro_concat_a_b(a, b)
#define ZMAKESTR(s)								__zpd_macro_make_string(s)


#endif