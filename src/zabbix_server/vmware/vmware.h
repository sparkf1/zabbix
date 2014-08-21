/*
** Zabbix
** Copyright (C) 2001-2014 Zabbix SIA
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/
#ifndef ZABBIX_VMWARE_H
#define ZABBIX_VMWARE_H

#include "common.h"

/* the vmware service state */
#define ZBX_VMWARE_STATE_NEW		0x001
#define ZBX_VMWARE_STATE_READY		0x002
#define ZBX_VMWARE_STATE_FAILED		0x004

#define ZBX_VMWARE_STATE_MASK		0x0FF

#define ZBX_VMWARE_STATE_UPDATING	0x100
#define ZBX_VMWARE_STATE_UPDATING_PERF	0x200

/* an entity monitored with performance counters */
typedef struct
{
	/* entity type: HostSystem or VirtualMachine */
	char			*type;

	/* entity id */
	char			*id;

	/* the peformance counte refresh rate */
	int			refresh;

	/* timestamp when the entity was queried last time */
	int			last_seen;

	/* the performance counters to monitor */
	zbx_vector_uint64_t	counters;
}
zbx_vmware_perf_entity_t;

typedef struct
{
	char	*name;
	char	*uuid;
}
zbx_vmware_datastore_t;

typedef struct
{
#define ZBX_VMWARE_DEV_TYPE_NIC		1
#define ZBX_VMWARE_DEV_TYPE_DISK	2
	int			type;
	char			*instance;
	char			*label;
}
zbx_vmware_dev_t;

/* the vmware virtual machine data */
typedef struct
{
	char			*uuid;
	char			*id;
	char			*details;
	zbx_vector_ptr_t	devs;
}
zbx_vmware_vm_t;

/* the vmware hypervisor data */
typedef struct
{
	char			*uuid;
	char			*id;
	char			*details;
	char			*clusterid;
	zbx_vector_ptr_t	datastores;
	zbx_vector_ptr_t	vms;
}
zbx_vmware_hv_t;

/* the vmware cluster data */
typedef struct
{
	char			*id;
	char			*name;
	char			*status;
}
zbx_vmware_cluster_t;

/* the vmware service data object */
typedef struct
{
	char	*error;
	char	*events;

	zbx_vector_ptr_t	hvs;
	zbx_vector_ptr_t	clusters;
}
zbx_vmware_data_t;

/* the vmware service data */
typedef struct
{
	char			*url;
	char			*username;
	char			*password;

	/* the service type - vCenter or vSphere */
	unsigned char		type;

	/* the service state - see ZBX_VMWARE_STATE_* defines */
	int			state;

	int			lastcheck;
	int			lastperfcheck;

	/* The last vmware service access time. If a service is not accessed for a day it is removed */
	int			lastaccess;

	/* the vmware service instance contents */
	char			*contents;

	/* the performance counter values */
	char			*stats;

	/* the performance counters */
	zbx_hashset_t 		counters;

	/* list of entities to monitor with performance counters */
	zbx_vector_ptr_t	perf_entities;

	/* list of entities monitored during last performance update*/
	zbx_vector_ptr_t	stats_entities;

	/* The service data object that is swapped with a new one during service update */
	zbx_vmware_data_t	*data;
}
zbx_vmware_service_t;

/* the vmware collector data */
typedef struct
{
	zbx_vector_ptr_t	services;
}
zbx_vmware_t;

/* the vmware collector statistics */
typedef struct
{
	zbx_uint64_t	memory_used;
	zbx_uint64_t	memory_total;
}
zbx_vmware_stats_t;

void	main_vmware_loop(void);

void	zbx_vmware_init(void);
void	zbx_vmware_destroy(void);

void	zbx_vmware_lock(void);
void	zbx_vmware_unlock(void);

zbx_vmware_service_t	*zbx_vmware_get_service(const char* url, const char* username, const char* password);
int	zbx_vmware_get_statistics(zbx_vmware_stats_t *stats);

int	zbx_vmware_service_get_perfcounterid(zbx_vmware_service_t *service, const char *path, zbx_uint64_t *counterid);
int	zbx_vmware_service_start_monitoring(zbx_vmware_service_t *service, const char *type, const char *id,
		zbx_uint64_t counterid);

/*
 * XML support
 */

#if defined(HAVE_LIBXML2) && defined(HAVE_LIBCURL)

#	define ZBX_XPATH_LN(LN)			"/*[local-name()='" LN "']"
#	define ZBX_XPATH_LN1(LN1)		"/" ZBX_XPATH_LN(LN1)
#	define ZBX_XPATH_LN2(LN1, LN2)		"/" ZBX_XPATH_LN(LN1) ZBX_XPATH_LN(LN2)
#	define ZBX_XPATH_LN3(LN1, LN2, LN3)	"/" ZBX_XPATH_LN(LN1) ZBX_XPATH_LN(LN2) ZBX_XPATH_LN(LN3)

char	*zbx_xml_read_value(const char *data, const char *xpath);
char	*zbx_xml_read_node_value(xmlDoc *doc, xmlNode *node, const char *xpath);
int	zbx_xml_read_values(const char *data, const char *xpath, zbx_vector_str_t *values);

#endif

#endif	/* ZABBIX_VMWARE_H */
