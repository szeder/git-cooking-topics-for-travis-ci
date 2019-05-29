/*
 * Copyright (c) 2010-2011 Ævar Arnfjörð Bjarmason
 *
 * This is a skeleton no-op implementation of gettext for Git.
 * You can replace it with something that uses libintl.h and wraps
 * gettext() to try out the translations.
 */

#ifndef GETTEXT_H
#define GETTEXT_H

#if defined(_) || defined(Q_)
#error "namespace conflict: '_' or 'Q_' is pre-defined?"
#endif

#ifndef NO_GETTEXT
#	include <libintl.h>
#else
#	ifdef gettext
#		undef gettext
#	endif
#	define gettext(s) (s)
#	ifdef ngettext
#		undef ngettext
#	endif
#	define ngettext(s, p, n) ((n == 1) ? (s) : (p))
#endif

#define FORMAT_PRESERVING(n) __attribute__((format_arg(n)))

#ifndef NO_GETTEXT
extern void git_setup_gettext(void);
extern int gettext_width(const char *s);
#else
static inline void git_setup_gettext(void)
{
}
static inline int gettext_width(const char *s)
{
	return strlen(s);
}
#endif

#ifdef GETTEXT_POISON
enum poison_mode {
	poison_mode_uninitialized = -1,
	poison_mode_none = 0,
	poison_mode_default,
	poison_mode_scrambled
};

extern enum poison_mode use_gettext_poison(void);
extern const char *gettext_scramble(const char *msg);

#define GETTEXT_POISON_MAGIC "# GETTEXT POISON #"
#endif

static inline FORMAT_PRESERVING(1) const char *_(const char *msgid)
{
	if (!*msgid)
		return "";
#ifdef GETTEXT_POISON
	if (use_gettext_poison() == poison_mode_default)
		return GETTEXT_POISON_MAGIC;
	else if (use_gettext_poison() == poison_mode_scrambled)
		return gettext_scramble(gettext(msgid));
#endif
	return gettext(msgid);
}

static inline FORMAT_PRESERVING(1) FORMAT_PRESERVING(2)
const char *Q_(const char *msgid, const char *plu, unsigned long n)
{
	const char *msg = ngettext(msgid, plu, n);
#ifdef GETTEXT_POISON
	if (use_gettext_poison() == poison_mode_default)
		return GETTEXT_POISON_MAGIC;
	else if (use_gettext_poison() == poison_mode_scrambled)
		return gettext_scramble(msg);
#endif
	return msg;
}

/* Mark msgid for translation but do not translate it. */
#if !USE_PARENS_AROUND_GETTEXT_N
#define N_(msgid) msgid
#else
/*
 * Strictly speaking, this will lead to invalid C when
 * used this way:
 *	static const char s[] = N_("FOO");
 * which will expand to
 *	static const char s[] = ("FOO");
 * and in valid C, the initializer on the right hand side must
 * be without the parentheses.  But many compilers do accept it
 * as a language extension and it will allow us to catch mistakes
 * like:
 *	static const char *msgs[] = {
 *		N_("one")
 *		N_("two"),
 *		N_("three"),
 *		NULL
 *	};
 * (notice the missing comma on one of the lines) by forcing
 * a compilation error, because parenthesised ("one") ("two")
 * will not get silently turned into ("onetwo").
 */
#define N_(msgid) (msgid)
#endif

const char *get_preferred_languages(void);
extern int is_utf8_locale(void);

#endif
