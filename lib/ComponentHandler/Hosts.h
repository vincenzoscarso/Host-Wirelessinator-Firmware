#ifndef HOSTS_H
#define HOSTS_H

#include <Host.h>
#include <vector>

namespace Hosts {
extern std::vector<Host> hosts;
bool updateHostsVector();
int getNumberOfHosts();
bool isHostNameValid(const std::string& host_name);
int getHostVectorIndexFromHostName(const std::string& host_name);
}

#endif /* HOSTS_H */