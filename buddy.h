/*
 * Copyright (c) 2020. Wuklab. All rights reserved.
 */

#ifndef _BUDDY_
#define _BUDDY_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sys/sysinfo.h>

#include "include/list.h"
#include "include/bitops.h"
#include "include/compiler.h"

/* Valid range [0, MAX_ORDER) */
#define MAX_ORDER		(11)
#define MAX_ORDER_NR_PAGES	(1 << (MAX_ORDER - 1))

#define PAGE_SHIFT		(12)
#define PAGE_SIZE		(1UL << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))

#define PFN_ALIGN(x)	(((unsigned long)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)
#define PFN_UP(x)	(((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
#define PFN_DOWN(x)	((x) >> PAGE_SHIFT)
#define PFN_PHYS(x)	((unsigned long)(x) << PAGE_SHIFT)
#define PHYS_PFN(x)	((unsigned long)((x) >> PAGE_SHIFT))

#define for_each_order(order) \
	for ((order) = 0; (order) < MAX_ORDER; (order)++)

struct free_area {
	struct list_head	free_list;
	unsigned long		nr_free;
};

struct page {
	unsigned long flags;
	unsigned long private;
	struct list_head list;
};

struct buddy_zone {
	struct free_area	free_area[MAX_ORDER];
	struct page		*page_map;
	pthread_spinlock_t	lock;

	/* Stats */
	unsigned long		nr_free;
};

#define page_private(page)		((page)->private)
#define set_page_private(page, v)	((page)->private = (v))

static inline unsigned int page_order(struct page *page)
{
	/* PageBuddy() must be checked by the caller */
	return page_private(page);
}

enum pageflags {
	PG_buddy,
};

#define TEST_PAGE_FLAG(uname, lname)				\
static inline int Page##uname(const struct page *page)		\
{								\
	return test_bit(PG_##lname, &page->flags);		\
}

#define __SET_PAGE_FLAG(uname, lname)				\
static inline void __SetPage##uname(struct page *page)		\
{								\
	__set_bit(PG_##lname, &page->flags);			\
}

#define __CLEAR_PAGE_FLAG(uname, lname)				\
static inline void __ClearPage##uname(struct page *page)	\
{								\
	__clear_bit(PG_##lname, &page->flags);			\
}

#define __TEST_SET_FLAG(uname, lname)				\
static inline int __TestSetPage##uname(struct page *page)	\
{								\
	return __test_and_set_bit(PG_##lname, &page->flags);	\
}

#define __TEST_CLEAR_FLAG(uname, lname)				\
static inline int __TestClearPage##uname(struct page *page)	\
{								\
	return __test_and_clear_bit(PG_##lname, &page->flags);	\
}

#define PAGE_FLAG(uname, lname)					\
	TEST_PAGE_FLAG(uname, lname)				\
	__SET_PAGE_FLAG(uname, lname)				\
	__CLEAR_PAGE_FLAG(uname, lname)				\
	__TEST_SET_FLAG(uname, lname)				\
	__TEST_CLEAR_FLAG(uname, lname)

PAGE_FLAG(Buddy, buddy)

#if 1
# define dprintf_DEBUG(fmt, ...) \
	printf("\033[1;34m[%s/%s()/%d]: " fmt "\033[0m", __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
# define dprintf_DEBUG(fmt, ...)  do { } while (0)
#endif

/* General info, always on */
#define dprintf_INFO(fmt, ...) \
	printf("\033[1;34m[%s/%s()/%d]: " fmt "\033[0m", __FILE__, __func__, __LINE__, __VA_ARGS__)

/* ERROR/WARNING info, always on */
#define dprintf_ERROR(fmt, ...) \
	printf("\033[1;31m[%s/%s()/%d]: " fmt "\033[0m", __FILE__, __func__, __LINE__, __VA_ARGS__)

#endif /* _BUDDY_ */
