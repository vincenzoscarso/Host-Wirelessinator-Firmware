#ifndef HOSTS_H
#define HOSTS_H

#include <Host.h>
#include <vector>

namespace Hosts {
extern std::vector<Host> hosts;
bool updateHostsVector();
int getNumberOfHosts();
}

#endif /* HOSTS_H */