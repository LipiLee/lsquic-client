/* Copyright (c) 2017 LiteSpeed Technologies Inc.  See LICENSE. */
/*
 * lsquic_cubic.h -- CUBIC congestion control protocol.
 */

#ifndef LSQUIC_CUBIC_H
#define LSQUIC_CUBIC_H 1

struct lsquic_cubic {
    lsquic_time_t   cu_min_delay;
    lsquic_time_t   cu_epoch_start;
    double          cu_K;
    unsigned long   cu_origin_point;
    unsigned long   cu_last_max_cwnd;
    unsigned long   cu_cwnd;
    unsigned long   cu_tcp_cwnd;
    unsigned long   cu_ssthresh;
    lsquic_cid_t    cu_cid;            /* Used for logging */
    enum cubic_flags {
        CU_TCP_FRIENDLY = (1 << 0),
    }               cu_flags;
    unsigned        cu_sampling_rate;
    lsquic_time_t   cu_last_logged;
};

#define DEFAULT_CUBIC_FLAGS (CU_TCP_FRIENDLY)

#define TCP_MSS 1460

void
lsquic_cubic_init_ext (struct lsquic_cubic *, lsquic_cid_t, enum cubic_flags);

#define lsquic_cubic_init(cubic, cid) \
            lsquic_cubic_init_ext(cubic, cid, DEFAULT_CUBIC_FLAGS)

void
lsquic_cubic_ack (struct lsquic_cubic *cubic, lsquic_time_t now,
                  lsquic_time_t rtt, int app_limited, unsigned n_bytes);

void
lsquic_cubic_loss (struct lsquic_cubic *cubic);

void
lsquic_cubic_timeout (struct lsquic_cubic *cubic);

void
lsquic_cubic_was_quiet (struct lsquic_cubic *, lsquic_time_t now);

#define lsquic_cubic_get_cwnd(c) (+(c)->cu_cwnd)

#define lsquic_cubic_in_slow_start(cubic) \
                        ((cubic)->cu_cwnd < (cubic)->cu_ssthresh)

#endif
