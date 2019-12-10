#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define MY_DEST_MAC0    0x08
#define MY_DEST_MAC1    0x00
#define MY_DEST_MAC2    0x27
#define MY_DEST_MAC3    0xcd
#define MY_DEST_MAC4    0xbf
#define MY_DEST_MAC5    0x05

#define DEFAULT_IF "wlp1s0"

//2nd
unsigned short csum (unsigned short *buf, int nwords);
uint16_t udp_checksum(const struct iphdr  *ip,
        const struct udphdr *udp,
        const uint16_t *buf);

 struct pseudo_hdr {
    u_int32_t source;
    u_int32_t dest;
    u_int8_t zero; //reserved, check http://www.rhyshaden.com/udp.htm
    u_int8_t protocol;
    u_int16_t udp_length;
};
 
uint16_t udp_checksum(const struct iphdr  *ip,
        const struct udphdr *udp,
        const uint16_t *buf)
{
    //take in account padding if necessary
    int calculated_length = ntohs(udp->len)%2 == 0 ? ntohs(udp->len) : ntohs(udp->len) + 1;
    struct pseudo_hdr ps_hdr = {0};
    bzero (&ps_hdr, sizeof(struct pseudo_hdr));
    uint8_t data[sizeof(struct pseudo_hdr) + calculated_length];
    bzero (data, sizeof(struct pseudo_hdr) + calculated_length );
 
    ps_hdr.source = ip->saddr;
    ps_hdr.dest = ip->daddr;
    ps_hdr.protocol = IPPROTO_UDP; //17
    ps_hdr.udp_length = udp->len;
 
    memcpy(data, &ps_hdr, sizeof(struct pseudo_hdr));
    memcpy(data + sizeof(struct pseudo_hdr), buf, ntohs(udp->len) ); //the remaining bytes are set to 0
 
    return csum((uint16_t *)data, sizeof(data)/2);
}
 
/* Not my code */
unsigned short csum (unsigned short *buf, int nwords)
{
    unsigned long sum;
 
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
 
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}

void UDPRecv(char *RBuff, int RPORT, char *RecvIP){
 int sockfd;
 char Packet[512];
 struct sockaddr_in my_addr;
 struct sockaddr_in their_addr;
 int RSPORT = 0;
 int addr_len, numbytes;
 
 if ((sockfd = socket(AF_INET, SOCK_DGRAM, 17)) == -1) {
 perror("socket");
 exit(1);
 }
 
 my_addr.sin_family = AF_INET;
 my_addr.sin_port = htons(RPORT);
 my_addr.sin_addr.s_addr = INADDR_ANY;
 bzero(&(my_addr.sin_zero), 8);
 
 if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
 perror("bind");
 exit(1);
 }
 
 addr_len = sizeof(struct sockaddr);
 
 if ((numbytes=recvfrom(sockfd, Packet, 1024, 0, (struct sockaddr *)&their_addr, (socklen_t *)&addr_len)) == -1) {
 perror("recvfrom");
 exit(1);
 }
  printf("recibido %s:%d[%s(%d)] \n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), Packet, strlen(Packet));
 close(sockfd);
 }
 
 

void UDPSend(char *DESTp, int PSOURCE, int PDEST, char *BUFFER, int sizebuff){
    char * SOURCE = "0.0.0.0";
    char * DEST = DESTp;
    int s;
    struct sockaddr_in daddr, saddr;
    struct ifreq if_idx;
    struct ifreq if_mac;
    char packet[512];
        char ifName[IFNAMSIZ];
    struct ether_header *eth = (struct ether_header *) packet;
    struct iphdr *ip = (struct iphdr *) (packet + sizeof(struct ether_header));
    struct udphdr *udp = (struct udphdr *)((void *) ip + sizeof(struct iphdr) + sizeof(struct ether_header));
       struct sockaddr_ll socket_address;
    if ((s = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }
strcpy(ifName, DEFAULT_IF);
 memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(s, SIOCGIFINDEX, &if_idx) < 0)
        perror("SIOCGIFINDEX");
    /* Get the MAC address of the interface to send on */
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(s, SIOCGIFHWADDR, &if_mac) < 0)
        perror("SIOCGIFHWADDR");

    /* Construct the Ethernet header */
    memset(packet, 0, 512);
    eth->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
    eth->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
    eth->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
    eth->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
    eth->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
    eth->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
    eth->ether_dhost[0] = 0xF4;
    eth->ether_dhost[1] = 0xF4;
    eth->ether_dhost[2] = 0xF4;
    eth->ether_dhost[3] = 0xF4;
    eth->ether_dhost[4] = 0xF4;
    eth->ether_dhost[5] = 0xF5;
    /* Ethertype field */
    eth->ether_type = htons(ETH_P_IP);


    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    /* Address length*/
    socket_address.sll_halen = ETH_ALEN;
    /* Destination MAC */
    socket_address.sll_addr[0] = MY_DEST_MAC0;
    socket_address.sll_addr[1] = MY_DEST_MAC1;
    socket_address.sll_addr[2] = MY_DEST_MAC2;
    socket_address.sll_addr[3] = MY_DEST_MAC3;
    socket_address.sll_addr[4] = MY_DEST_MAC4;
    socket_address.sll_addr[5] = MY_DEST_MAC5;


   // daddr.sin_family = AF_INET;
   // saddr.sin_family = AF_INET;
    daddr.sin_port = htons(PDEST);
    saddr.sin_port = htons(PSOURCE);
    inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
    inet_pton(AF_INET, SOURCE, (struct in_addr *)&saddr.sin_addr.s_addr);
    memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
    memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));
    memset(udp, 0, sizeof(struct udphdr));
    memset(ip, 0, sizeof(struct iphdr));
    ip->ihl = 5; //header length
    ip->version = 4;
    ip->tos = 0x0;
    ip->id = 20000 + (rand() % 20000);
    ip->frag_off = 0; 
    ip->ttl = 60 + (rand() % 60);          /* default value */
    ip->protocol = 17;  //IPPROTO_RAW;  /* protocol at L4 */
    ip->check = 0;          /* not needed in iphdr */
    ip->saddr = saddr.sin_addr.s_addr;
    ip->daddr = daddr.sin_addr.s_addr;
    udp->source = htons(PSOURCE);
    udp->dest = htons (PDEST);
    char incoming[sizebuff];
    char * packet_pos  = ((void *) udp) + sizeof(struct udphdr);
    memset(((void *) udp) + sizeof(struct udphdr), 'A', sizebuff);
    int sizeudpdata = sizeof(struct udphdr) + sizebuff;
    ip->tot_len = htons(sizeudpdata + sizeof(struct iphdr));    /* 16 byte value */
    udp->len = htons(sizeudpdata);
    int optval = 1;
    if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int))
            < 0)
        perror ("IP HDRINCL");
        int sizepacket = sizeof(struct ether_header) + sizeof(struct iphdr) + sizeudpdata;
    udp->check = 0;
    	for(int i = 0; i != sizebuff; i++) packet_pos[i] = BUFFER[i];
   memcpy(((void *) udp) + sizeof(struct udphdr), (char*)packet_pos, sizebuff);
    udp->check = udp_checksum(
            ip,
            udp,
           (const uint16_t *)udp);

sendto(s, (char *)packet, sizepacket, 0,(struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) ;     
    close(s);
}



