/*
 * OpenWFD - Open-Source Wifi-Display Implementation
 *
 * Copyright (c) 2013 David Herrmann <dh.herrmann@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef OWFD_P2PD_H
#define OWFD_P2PD_H

#include <stdbool.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/signalfd.h>
#include "wpa.h"

#ifdef __cplusplus
extern "C" {
#endif

/* config handling */

struct owfd_p2pd_config {
	unsigned int verbose : 1;
	unsigned int silent : 1;
	unsigned int debug : 1;

	char *interface;

	char *wpa_binary;
	char *wpa_ctrldir;
};

void owfd_p2pd_init_config(struct owfd_p2pd_config *conf);
void owfd_p2pd_clear_config(struct owfd_p2pd_config *conf);
int owfd_p2pd_parse_argv(struct owfd_p2pd_config *conf, int argc, char **argv);

/* main */

enum owfd_p2pd_ep_action {
	OWFD_P2PD_EP_HANDLED,
	OWFD_P2PD_EP_NOT_HANDLED,
	OWFD_P2PD_EP_QUIT,
};

struct owfd_p2pd_ep {
	struct epoll_event *ev;
	struct epoll_event *evs;
	size_t num;
};

int owfd_p2pd_ep_add(int efd, int *fd, unsigned int events);
void owfd_p2pd_ep_update(int efd, int *fd, unsigned int events);
void owfd_p2pd_ep_remove(int efd, int fd);

/* interface handling */

struct owfd_p2pd_interface;

typedef void (*owfd_p2pd_interface_event_fn) (struct owfd_p2pd_interface *ifc,
					      struct owfd_wpa_event *ev,
					      void *data);

int owfd_p2pd_interface_new(struct owfd_p2pd_interface **out,
			    struct owfd_p2pd_config *conf, int efd);
void owfd_p2pd_interface_free(struct owfd_p2pd_interface *iface);
int owfd_p2pd_interface_dispatch(struct owfd_p2pd_interface *iface,
				 struct owfd_p2pd_ep *ep);
int owfd_p2pd_interface_dispatch_chld(struct owfd_p2pd_interface *iface,
				      struct signalfd_siginfo *info);

int owfd_p2pd_interface_register_event_fn(struct owfd_p2pd_interface *iface,
					  owfd_p2pd_interface_event_fn event_fn,
					  void *data);
void owfd_p2pd_interface_unregister_event_fn(struct owfd_p2pd_interface *iface,
					     owfd_p2pd_interface_event_fn event_fn,
					     void *data);

int owfd_p2pd_interface_connect(struct owfd_p2pd_interface *iface,
				const char *peer_mac,
				const char *pin,
				const char *pin_mode);

/* dummy */

struct owfd_p2pd_dummy;

int owfd_p2pd_dummy_new(struct owfd_p2pd_dummy **out,
			struct owfd_p2pd_config *config,
			struct owfd_p2pd_interface *iface);
void owfd_p2pd_dummy_free(struct owfd_p2pd_dummy *dummy);

#ifdef __cplusplus
}
#endif

#endif /* OPENWFD_P2PD_H */
