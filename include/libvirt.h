/*
 * libvirt.h:
 * Summary: core interfaces for the libvirt library
 * Description: Provides the interfaces of the libvirt library to handle
 *              Xen domains from a process running in domain 0
 *
 * Copy:  Copyright (C) 2005,2006 Red Hat, Inc.
 *
 * See COPYING.LIB for the License of this software
 *
 * Author: Daniel Veillard <veillard@redhat.com>
 */

#ifndef __VIR_VIRLIB_H__
#define __VIR_VIRLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * virConnect:
 *
 * a virConnect is a private structure representing a connection to
 * the Xen Hypervisor.
 */
typedef struct _virConnect virConnect;

/**
 * virConnectPtr:
 *
 * a virConnectPtr is pointer to a virConnect private structure, this is the
 * type used to reference a connection to the Xen Hypervisor in the API.
 */
typedef virConnect *virConnectPtr;

/**
 * virDomain:
 *
 * a virDomain is a private structure representing a Xen domain.
 */
typedef struct _virDomain virDomain;

/**
 * virDomainPtr:
 *
 * a virDomainPtr is pointer to a virDomain private structure, this is the
 * type used to reference a Xen domain in the API.
 */
typedef virDomain *virDomainPtr;

/**
 * virDomainState:
 *
 * A domain may be in different states at a given point in time
 */
typedef enum {
     VIR_DOMAIN_NOSTATE	= 0, /* no state */
     VIR_DOMAIN_RUNNING	= 1, /* the domain is running */
     VIR_DOMAIN_BLOCKED	= 2, /* the domain is blocked on resource */
     VIR_DOMAIN_PAUSED	= 3, /* the domain is paused by user */
     VIR_DOMAIN_SHUTDOWN= 4, /* the domain is being shut down */
     VIR_DOMAIN_SHUTOFF	= 5, /* the domain is shut off */
     VIR_DOMAIN_CRASHED = 6  /* the domain is crashed */
} virDomainState;

/**
 * virDomainRestart:
 *
 * Flags that determine the action to take on a shutdown or crash of a domain
 */
typedef enum {
     VIR_DOMAIN_DESTROY	= 1, /* destroy the domain */
     VIR_DOMAIN_RESTART	= 2, /* restart the domain */
     VIR_DOMAIN_PRESERVE= 3, /* keep as is, need manual destroy, for debug */
     VIR_DOMAIN_RENAME_RESTART= 4/* restart under an new unique name */
} virDomainRestart;

/**
 * virDeviceMode:
 *
 * Flags that determine permission to expose a device to the guest
 */
typedef enum {
     VIR_DEVICE_DEFAULT	= 0, /* Default mode */
     VIR_DEVICE_RO	= 1, /* Access read-only */
     VIR_DEVICE_RW	= 2, /* Access read-write */
     VIR_DEVICE_RW_FORCE= 3  /* Forced read-write even if already used */
} virDeviceMode;

/**
 * virDomainInfoPtr:
 *
 * a virDomainInfo is a structure filled by virDomainGetInfo() and extracting
 * runtime informations for a given active Domain
 */

typedef struct _virDomainInfo virDomainInfo;

struct _virDomainInfo {
    unsigned char state;	/* the running state, one of virDomainFlags */
    unsigned long maxMem;	/* the maximum memory in KBytes allowed */
    unsigned long memory;	/* the memory in KBytes used by the domain */
    unsigned short nrVirtCpu;	/* the number of virtual CPUs for the domain */
    unsigned long long cpuTime;	/* the CPU time used in nanoseconds */
};

/**
 * virDomainInfoPtr:
 *
 * a virDomainInfoPtr is a pointer to a virDomainInfo structure.
 */

typedef virDomainInfo *virDomainInfoPtr;

/**
 * virDomainKernel:
 *
 * a virDomainImage is the set of kernel related informations associated
 * to a domain
 */

typedef struct _virDomainKernel virDomainKernel;

struct _virDomainKernel {
    const char *kernel;		/* filename pointing to the kernel image */
    const char *ramdisk;	/* an optional init ramdisk */
    const char *root;		/* an optional root block device */
    const char *extra;		/* optional kernel command line parameters */
};

/**
 * virDomainKernelPtr:
 *
 * a virDomainKernelPtr is a pointer to a virDomainKernel structure.
 */

typedef virDomainKernel *virDomainKernelPtr;

/**
 * virDomainCreateFlags:
 *
 * Flags OR'ed together to provide specific behaviour when creating a
 * Domain.
 */
typedef enum {
     VIR_DOMAIN_NONE = 0
} virDomainCreateFlags;

/**
 * virNodeInfoPtr:
 *
 * a virNodeInfo is a structure filled by virNodeGetInfo() and providing
 * the informations for the Node. 
 */

typedef struct _virNodeInfo virNodeInfo;

struct _virNodeInfo {
    char model[32];	/* string indicating the CPU model */
    unsigned long memory;/* memory size in kilobytes */
    unsigned int cpus;	/* the number of active CPUs */
    unsigned int mhz;	/* expected CPU frequency */
    unsigned int nodes;	/* the number of NUMA cell, 1 for uniform mem access */
    unsigned int sockets;/* number of CPU socket per node */
    unsigned int cores;	/* number of core per socket */
    unsigned int threads;/* number of threads per core */
};

/**
 * virNodeInfoPtr:
 *
 * a virNodeInfoPtr is a pointer to a virNodeInfo structure.
 */

typedef virNodeInfo *virNodeInfoPtr;

/* library versionning */

/**
 * LIBVIR_VERSION_NUMBER:
 *
 * Macro providing the version of the library as 
 * version * 1,000,000 + minor * 1000 + micro
 */

#define LIBVIR_VERSION_NUMBER 1000

int			virGetVersion		(unsigned long *libVer,
						 const char *type,
						 unsigned long *typeVer);

/*
 * Connection and disconnections to the Hypervisor
 */
int			virInitialize		(void);

virConnectPtr		virConnectOpen		(const char *name);
virConnectPtr		virConnectOpenReadOnly	(const char *name);
int			virConnectClose		(virConnectPtr conn);
const char *		virConnectGetType	(virConnectPtr conn);
int			virConnectGetVersion	(virConnectPtr conn,
						 unsigned long *hvVer);
int			virNodeGetInfo		(virConnectPtr conn,
						 virNodeInfoPtr info);

/*
 * Gather list of running domains
 */
int			virConnectListDomains	(virConnectPtr conn,
						 int *ids,
						 int maxids);

/*
 * Number of domains
 */
int			virConnectNumOfDomains	(virConnectPtr conn);


/*
 * Domain creation and destruction
 */
virDomainPtr		virDomainCreateLinux	(virConnectPtr conn,
						 const char *xmlDesc,
						 unsigned int flags);
virDomainPtr		virDomainLookupByName	(virConnectPtr conn,
						 const char *name);
virDomainPtr		virDomainLookupByID	(virConnectPtr conn,
						 int id);
virDomainPtr		virDomainLookupByUUID	(virConnectPtr conn,
						 const unsigned char *uuid);
int			virDomainShutdown	(virDomainPtr domain);
int			virDomainReboot		(virDomainPtr domain,
						 unsigned int flags);
int			virDomainDestroy	(virDomainPtr domain);
int			virDomainFree		(virDomainPtr domain);

/*
 * Domain suspend/resume
 */
int			virDomainSuspend	(virDomainPtr domain);
int			virDomainResume		(virDomainPtr domain);

/*
 * Domain save/restore
 */
int			virDomainSave		(virDomainPtr domain,
						 const char *to);
int			virDomainRestore	(virConnectPtr conn,
						 const char *from);

/*
 * Domain runtime informations
 */
int			virDomainGetInfo	(virDomainPtr domain,
						 virDomainInfoPtr info);
						 
/*
 * Dynamic control of domains
 */
const char *		virDomainGetName	(virDomainPtr domain);
unsigned int		virDomainGetID		(virDomainPtr domain);
int			virDomainGetUUID	(virDomainPtr domain,
						 unsigned char *uuid);
char *			virDomainGetOSType	(virDomainPtr domain);
unsigned long		virDomainGetMaxMemory	(virDomainPtr domain);
int			virDomainSetMaxMemory	(virDomainPtr domain,
						 unsigned long memory);
int			virDomainSetMemory	(virDomainPtr domain,
						 unsigned long memory);
/*
 * XML domain description
 */
char *			virDomainGetXMLDesc	(virDomainPtr domain,
						 int flags);

#ifdef __cplusplus
}
#endif

#endif /* __VIR_VIRLIB_H__ */
