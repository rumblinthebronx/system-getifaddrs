#include "ruby.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get_if_family(struct ifaddrs *ifa){
	if(ifa && ifa->ifa_addr)
		return ifa->ifa_addr->sa_family;
	return 0;
}

char * get_if_name(struct ifaddrs *ifa){
	return ifa->ifa_name;
}

int get_if_host(struct ifaddrs *ifa, char *host){
    int family = get_if_family(ifa);
	if(getnameinfo(ifa->ifa_addr, 
        (family == AF_INET) ? sizeof(struct sockaddr_in) :
        sizeof(struct sockaddr_in6),
	    host, NI_MAXHOST,
		NULL, 0, NI_NUMERICHOST))
		return 0;
	return 1;
}

int get_if_netmask(struct ifaddrs *ifa, char *netmask){
    int family = get_if_family(ifa);
	if(getnameinfo(ifa->ifa_netmask, 
        (family == AF_INET) ? sizeof(struct sockaddr_in) :
        sizeof(struct sockaddr_in6),
		netmask, NI_MAXHOST,
		NULL, 0, NI_NUMERICHOST))
		return 0;
	return 1;
}

VALUE set_if_hash(VALUE rb_if_hash, struct ifaddrs *ifa, int family){
    char *if_host, *if_netmask, *if_name;
    if_name = get_if_name(ifa);
    if_host = malloc(sizeof(char) * NI_MAXHOST);
    if (! get_if_host(ifa, if_host))
        rb_raise(rb_eSystemCallError, "Can't get IP from %s", if_name);

    if_netmask = malloc(sizeof(char) * NI_MAXHOST);
    if (! get_if_netmask(ifa, if_netmask))
        rb_raise(rb_eSystemCallError, "Can't get IP from %s", if_name);

    char *str_inet_name = "inet_addr_v4";
    char *str_inet_name6 = "inet_addr_v6";
    char *str_netmask_name = "netmask_v4";
    char *str_netmask_name6 = "netmask_v6";
    VALUE rb_if_data_hash = rb_hash_aref(rb_if_hash, rb_str_intern(rb_str_new2(if_name)));

    if (rb_if_data_hash == Qnil) {
        rb_if_data_hash = rb_hash_new();
        rb_hash_aset(rb_if_hash,
            rb_str_intern(rb_str_new2(if_name)),
            rb_if_data_hash);
    }

    rb_hash_aset(rb_if_data_hash,
        rb_str_intern(rb_str_new2((family == AF_INET) ? str_inet_name : str_inet_name6)),
        rb_str_new2(if_host));
    rb_hash_aset(rb_if_data_hash,
        rb_str_intern(rb_str_new2((family == AF_INET) ? str_netmask_name : str_netmask_name6)),
        rb_str_new2(if_netmask));
    return rb_if_data_hash;
}

VALUE rb_get_ifaddrs(void)
{
    struct ifaddrs *ifaddr, *ifa;
    VALUE rb_if_hash;

    if (getifaddrs(&ifaddr) == -1)
    {
        rb_raise(rb_eSystemCallError, "Can't get info about interfaces");
    }
    rb_if_hash = rb_hash_new();
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        int family;

        family = get_if_family(ifa);
        if (family == AF_INET || family == AF_INET6)
        {
            set_if_hash(rb_if_hash, ifa, family);  
        }
        }
        freeifaddrs(ifaddr);
        return rb_if_hash;
}

VALUE mSystem;
void Init_rb_getifaddrs(){
    mSystem = rb_define_module("System");
    rb_define_module_function(mSystem, "get_ifaddrs", rb_get_ifaddrs, 0);
}

