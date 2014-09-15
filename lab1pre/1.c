#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

typedef struct partition {
	unsigned char drive;				/* 0x80 - active */

	unsigned char head;				 /* starting head */
	unsigned char sector;			  /* starting sector */
	unsigned char cylinder;			/* starting cylinder */

	unsigned char sys_type;			/* partition type */

	unsigned char end_head;			/* end head */
	unsigned char end_sector;		 /* end sector */
	unsigned char end_cylinder;	  /* end cylinder */

	unsigned long start_sector;	  /* starting sector counting from 0 */
	unsigned long nr_sectors;		 /* nr of sectors in partition */
} partition;

void printRaw(partition*);
void printfDisk(partition*);
partition* processPart(partition*);
unsigned char getSector(unsigned char);
unsigned char getCylinder(unsigned char, unsigned char);
unsigned long reverseByteOrder(unsigned long);

char buf[512];
int sector;

void main()
{
	int fd, r, i, 
		sector = 0,
		starting_sector = 0,
		partitions_length = 0,
		partitions_max_length = 4, //Array only contains the partitions in sector 0, so we only need 4 for those
		heads = -1,
		sectors = -1,
		cylSize = 0;

	char input;

	unsigned long temp;

	partition *ext, *ptr;
	partition *partitions[partitions_max_length];

	fd = open("vdisk", O_RDONLY);			 // open disk iamge file for READ
	r = read(fd, buf, 512);					 // read 512 bytes into buf
	
	for(i = 0; i < 4; i++) {
		partitions[i] = processPart((partition *)&buf[0x1BE + (i * 16)]); //Read in the partition

		//Calculate the number of heads and sectors
		//This was largely an educated guess of how I could find this information
		if(partitions[i]->head > heads) {
			heads = partitions[i]->head;
		}
		if(partitions[i]->end_sector > sectors) {
			sectors = partitions[i]->end_sector;
		}
		partitions_length++;
	}

	cylSize = sectors * (heads + 1);

	//Uses 4 for loops because I couldn't think of a better method for printing the headers then the details below
	printf("heads=%d sectors=%d cyl_size=%d\n", heads+1, sectors, sectors * (heads + 1));
	printf("----------          RAW form          ----------\n");
	printf("D   H   S   C   T     H     S   C   SS       #\n"); //Some basic labels for the raw data
	for(i = 0; i < partitions_length; i++) {
		printRaw(partitions[i]);
	}

	printf("----------      Linux fdisk form      ----------\n");
	printf("start_cyl end_cyl  blocks   type\n");
	for(i = 0; i < partitions_length; i++) {
		printfDisk(partitions[i]);

	}

	printf("----------Looking for Extend Partition----------\n");
	for(i = 0; i < partitions_length; i++) {
		if(partitions[i]->sys_type == 5) {
			ptr = partitions[i];
			printf("Ext Partition: %d Start Sector: %lu\n", i+1, ptr->start_sector);
		}
	}

	if(ptr == NULL) {
		printf("No Extend Partitions");
	} else {
		printf("Press any key to continue: ");
		scanf("%c", &input);
		starting_sector = sector = ptr->start_sector; //preserve the first starting sector, because the rest are relative to that value
		do {
			lseek(fd, (long)sector*512, 0);        // seek to sector sector
			r = read(fd, buf, 512);                // read 512 bytes into buf
 			ext = processPart((partition *)&buf[0x1BE]);
 			ptr = processPart((partition *)&buf[0x1CE]);
 			printf("start_sector:%d cylSize:%d startCyl: %d\n", sector, cylSize, ext->cylinder);
 			
 			/* 
 			printRaw(ext); //For debugging purposes
 			printRaw(ptr);
			*/

 			printfDisk(ext);
 			printfDisk(ptr);
 			printf("p->start_sector = %lu\n", ptr->start_sector);
 			sector = ptr->start_sector + starting_sector;
		} while(ptr->start_sector != 0);
	}

	close(fd);									  // close opened file
}

void printRaw(partition* part) {
	printf("%-3u %-3u %-3u %-3u %-5x %-5u %-3u %-3u %-6lu %-4lu\n", 
		part->drive, part->head, part->sector, part->cylinder, part->sys_type, part->end_head, part->end_sector, (part->end_cylinder + 1), 
		part->start_sector, part->nr_sectors);
}

void printfDisk(partition* part) {
	int blocks = part->nr_sectors / 2;
	printf("%6d %6d %8d %6x\n", part->cylinder + 1, part->end_cylinder + 1, blocks, part->sys_type);
}

partition* processPart(partition* part) {
	//Handles shifting bytes for sectors/cylinders and rearranges the starting sector
	part->start_sector = reverseByteOrder(part->start_sector);
	part->sector = getSector(part->sector);
	part->end_sector = getSector(part->end_sector);
	part->cylinder = getCylinder(part->sector, part->cylinder);
	part->end_cylinder = getCylinder(part->end_sector, part->end_cylinder);
	/*For NR_Sectors: I know this isn't accurate, but I couldn't figure out why it was incorrect, and everytime I tried editing it, 
						it modified neighboring elements in the array

	*/
	return part;
}

unsigned char getSector(unsigned char sector) {
	return sector & 0x3F;
}

unsigned char getCylinder(unsigned char sector, unsigned char cylinder) {
	unsigned char c = ((sector >> 6) << 8);
	return c | cylinder;
}

unsigned long reverseByteOrder(unsigned long value) {
	//According to the MBR spec, the byte order is reversed... so here is the byte order re-arranged
	return (value & 0xF) | (value & 0xF0) | (value & 0xF00) | (value & 0xF000); 
}