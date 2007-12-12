/* pic.h
 *  Copyright (C) 2005, The Perl Foundation.
 *  SVN Info
 *     $Id$
 *  Overview:
 *     This is the api header for the pic subsystem
 *  Data Structure and Algorithms:
 *  History:
 *  Notes:
 *  References:
 */

#ifndef PARROT_PIC_H_GUARD
#define PARROT_PIC_H_GUARD

/*
 * one cache slot
 *
 * if types exceed 16 bits or for general MMD function calls an
 * extended cache slot is needed with more type entries
 */
typedef struct Parrot_pic_lru_t {
    union {
        INTVAL type;                    /* for MMD left << 16 | right type */
        PMC *signature;                 /* arg passing signature */
    } u;
    union {
        funcptr_t real_function;        /* the actual C code */
        PMC *sub;                       /* or a Sub PMC */
        PMC **pattr;                    /* attribute location */
    } f;
} Parrot_PIC_lru;

/*
 * PIC 3 more cache slots
 */
typedef struct Parrot_pic_t {
    Parrot_PIC_lru lru[3];              /* PIC - three more cache entries */
    INTVAL miss_count;                  /* how many misses */
} Parrot_PIC;

/*
 * the main used MIC one cache slot - 4 words size
 */
typedef struct Parrot_mic_t {
    Parrot_PIC_lru lru;                 /* MIC - one cache */
    union {
        STRING *method;                 /* for callmethod */
        INTVAL func_nr;                 /* MMD function number */
        STRING *attribute;              /* obj.attribute */
        PMC *sig;                       /* arg passing */
    } m;
    Parrot_PIC *pic;                    /* more cache entries */
} Parrot_MIC;

/*
 * memory is managed by this structure hanging off a
 * PackFile_ByteCode segment
 */
typedef struct Parrot_pic_store_t {
    struct Parrot_pic_store_t *prev;    /* prev pic_store */
    size_t usable;                      /* size of usable memory: */
    Parrot_PIC *pic;                    /* from rear */
    Parrot_MIC *mic;                    /* idx access to allocated MICs */
    size_t n_mics;                      /* range check, debugging mainly */
} Parrot_PIC_store;

typedef int (*arg_pass_f)(PARROT_INTERP, PMC *sig,
            char *src_base, void **src_pc, char *dest_base, void **dest_pc);

/* more or less private interfaces */

/* HEADERIZER BEGIN: src/pic.c */

PARROT_WARN_UNUSED_RESULT
PARROT_CANNOT_RETURN_NULL
Parrot_MIC* parrot_PIC_alloc_mic(const PARROT_INTERP, size_t n);

PARROT_WARN_UNUSED_RESULT
PARROT_CANNOT_RETURN_NULL
Parrot_PIC* parrot_PIC_alloc_pic(PARROT_INTERP)
        __attribute__nonnull__(1);

void parrot_PIC_alloc_store(NOTNULL(struct PackFile_ByteCode *cs), size_t n)
        __attribute__nonnull__(1);

PARROT_WARN_UNUSED_RESULT
int parrot_pic_check_sig(
    ARGIN(const PMC *sig1),
    ARGIN(const PMC *sig2),
    NOTNULL(int *type))
        __attribute__nonnull__(1)
        __attribute__nonnull__(2)
        __attribute__nonnull__(3);

void parrot_PIC_destroy(NOTNULL(struct PackFile_ByteCode *cs))
        __attribute__nonnull__(1);

void parrot_pic_find_infix_v_pp(PARROT_INTERP,
    NOTNULL(PMC *left),
    NOTNULL(PMC *right),
    NOTNULL(Parrot_MIC *mic),
    NOTNULL(opcode_t *cur_opcode))
        __attribute__nonnull__(1)
        __attribute__nonnull__(2)
        __attribute__nonnull__(3)
        __attribute__nonnull__(4)
        __attribute__nonnull__(5);

PARROT_CONST_FUNCTION
int parrot_PIC_op_is_cached(int op_code);

PARROT_WARN_UNUSED_RESULT
PARROT_CAN_RETURN_NULL
void * parrot_pic_opcode(PARROT_INTERP, INTVAL op)
        __attribute__nonnull__(1);

void parrot_PIC_prederef(PARROT_INTERP,
    opcode_t op,
    NOTNULL(void **pc_pred),
    int core)
        __attribute__nonnull__(1)
        __attribute__nonnull__(3);

/* HEADERIZER END: src/pic.c */


/* HEADERIZER BEGIN: src/pic_jit.c */

PARROT_WARN_UNUSED_RESULT
int parrot_pic_is_safe_to_jit(PARROT_INTERP,
    NOTNULL(PMC *sub),
    NOTNULL(PMC *sig_args),
    NOTNULL(PMC *sig_results),
    NOTNULL(int *flags))
        __attribute__nonnull__(1)
        __attribute__nonnull__(2)
        __attribute__nonnull__(3)
        __attribute__nonnull__(4)
        __attribute__nonnull__(5);

funcptr_t parrot_pic_JIT_sub(PARROT_INTERP, NOTNULL(PMC *sub), int flags)
        __attribute__nonnull__(1)
        __attribute__nonnull__(2);

/* HEADERIZER END: src/pic_jit.c */

#endif /* PARROT_PIC_H_GUARD */

/*
 * Local variables:
 *   c-file-style: "parrot"
 * End:
 * vim: expandtab shiftwidth=4:
 */
