#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <limits.h> 
#include <math.h>

int MSG_LEN =10000;
int lambda=10;
int new=1;
int method=0;

struct packet{
int no;
double intime;
double endtime;
int qno;
};

struct server{
int size;
double curtime;
};
struct server q1,q2;

double exponGenerator(double myLamda)
{
    double pv = 0.0;
	while (1)
	{
		pv = (double)rand() / (double)RAND_MAX;
		if (pv != 1)
			break;
	}
	pv = (-1.0 / myLamda) * log(1 - pv);
	return pv;
}
int randomSelection(){
if(rand()%2 ==0){
return 1;
}
else{
return 0;
}
}

int minQueue(){
if(q1.size > q2.size){
 return 1;
}
else{
return 0;
}
}

int main()
{
struct packet pac[MSG_LEN+1];
int i,que_len=0;
float block_no;
q1.size=0;
q2.size=0;
q1.curtime=0;
q2.curtime=0;
pac[0].intime = 0;
pac[0].endtime=0;
for(i=1;i<=MSG_LEN ; i++)
{
pac[i].intime = pac[i-1].intime + exponGenerator(lambda);

int j,no_serviced=0,no_entered=0;
for(j=0;j<i;j++){
if(pac[j].qno==1 && pac[j].endtime!=0 ){
no_entered++;
}
if(pac[j].qno==1 && pac[j].endtime <= pac[i].intime && pac[j].endtime!=0  ){
no_serviced++;
}
//printf("\n %d %d  for size ",no_entered, no_serviced);
}
q1.size=no_entered - no_serviced;

int no_serviced2=0,no_entered2=0;
for(j=0;j<i;j++){
if(pac[j].qno==2 && pac[j].endtime!=0 )
{
no_entered2++;
}
if(pac[j].qno==2 &&  pac[j].endtime <= pac[i].intime && pac[j].endtime!=0  )
{
//printf("yes");
no_serviced2++;
}
//printf("\n %d %d  for size ",no_entered, no_serviced);
}
q2.size=no_entered2 - no_serviced2;

if(method){
if(randomSelection()==1){
pac[i].qno=1;
}else{
pac[i].qno=2;
}
}else{
if(minQueue()==0){
pac[i].qno=1;
}else{
pac[i].qno=2;
}
}

if(pac[i].qno==1){

if(q1.size==0){
pac[i].endtime=pac[i].intime +exponGenerator(new);
q1.curtime=pac[i].endtime;
que_len += q1.size;
}
else if(q1.size<10){
pac[i].endtime=q1.curtime +exponGenerator(new);
q1.curtime=pac[i].endtime;
que_len += q1.size;
}
else{
block_no++;
pac[i].endtime=0;
que_len += 10;
}

}
else{

if(q2.size==0){
pac[i].endtime=pac[i].intime +exponGenerator(new);
q2.curtime=pac[i].endtime;
que_len += q2.size;
}
else if(q2.size<10){
pac[i].endtime=q2.curtime +exponGenerator(new);
q2.curtime=pac[i].endtime;
que_len += q2.size;
}
else{
block_no++;
pac[i].endtime=0;
que_len += 10;
}
}
//printf("\n %d %d %d %d %f %f",q1.size,q2.size,que_len,pac[i].qno,pac[i].intime,pac[i].endtime);
}
double sourjan=0;
for(i=1;i<=MSG_LEN ;i++){
if(pac[i].intime!=0 && pac[i].endtime !=0){
sourjan += pac[i].endtime - pac[i].intime;
}
}

printf("\n Blocking probability is %f",block_no/MSG_LEN);
printf("\n Average Que Len is %d " ,que_len/MSG_LEN);
printf("\n Average Sourjon Time is %f",sourjan/(MSG_LEN-block_no));
}