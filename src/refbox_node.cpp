#include "ros/ros.h"
#include "std_msgs/String.h"

#include "SSL_refbox_client.h"
#include <sstream>
#include <krssg_ssl_msgs/SSL_Refbox.h>
#include "referee.pb.h"
#include <ctime>
#include "timer.h"


using namespace std;


int main(int argc, char **argv)
{
	ros::init(argc, argv, "refbox_node");
	ros::NodeHandle n;
	
	ros::Publisher chatter_pub = n.advertise<krssg_ssl_msgs::SSL_Refbox>("/Refbox", 10009);//ros message advertise port
	ros::Rate loop_rate(60);

	
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	int port = 10003;//refbox UDP multicast port
	SSLRefboxClient client(port);	
	client.open(true);
	printf("Connected to refbox\n");
	SSL_Referee packet;
	while(ros::ok()) {
		if (client.receive(packet)) {
			krssg_ssl_msgs::SSL_Refbox msg;
			
			//Common data
			long long _packet_timestamp = packet.packet_timestamp();
			int _stage = packet.stage();
			if(packet.has_stage_time_left())
				long long _stage_time_left = packet.stage_time_left();
			

			msg.packet_timestamp = _packet_timestamp;
			msg.stage = _stage;
			msg.command = packet.command();
			msg.command_counter = packet.command_counter();
			msg.command_timestamp = packet.command_timestamp();
			if(packet.has_stage_time_left())
				msg.stage_time_left = packet.stage_time_left();

			


			SSL_Referee::TeamInfo _yellow = packet.yellow();
			SSL_Referee::TeamInfo _blue = packet.blue();


			//Blue team data
			msg.b_name = _blue.name();
			msg.b_score = _blue.score();
			msg.b_red_cards = _blue.red_cards();
			msg.b_yellow_cards = _blue.yellow_cards();
			msg.b_timeouts = _blue.timeouts();
			msg.b_timeout_time = _blue.timeout_time();
			msg.b_goalie = _blue.goalie();
			for(int i=0;i<_blue.yellow_cards();i++){
				msg.b_card_times.push_back(_blue.yellow_card_times(i));
			}

			//Yellow team data
			msg.y_name = _yellow.name();
			msg.y_score = _yellow.score();
			msg.y_red_cards = _yellow.red_cards();
			msg.y_yellow_cards = _yellow.yellow_cards();
			msg.y_timeouts = _yellow.timeouts();
			msg.y_timeout_time = _yellow.timeout_time();
			msg.y_goalie = _yellow.goalie();
			for(int i=0;i<_yellow.yellow_cards();i++){
				msg.y_card_times.push_back(_yellow.yellow_card_times(i));
			}


			cout<<"\n***************************************************************************************************";
			cout<<"\ntime= "<<_packet_timestamp;
			cout<<"\nCommand= "<<packet.command();
			cout<<"\nCommand counter= "<<packet.command_counter();
			cout<<"\nCommand timestamp= "<<packet.command_timestamp();
			
			cout<<"\nYellow team data :";
			cout<<"\n 	Name= "<<_yellow.name();
			cout<<"\n 	Score= "<<_yellow.score();
			cout<<"\n 	Red Cards= "<<_yellow.red_cards();
			cout<<"\n 	Yellow Cards= "<<_yellow.yellow_cards();
			cout<<"\n 	Timeouts= "<<_yellow.timeouts();
			cout<<"\n 	Goalie= "<<_yellow.goalie();

			cout<<"\nBlue team data :";
			cout<<"\n 	Name= "<<_blue.name();
			cout<<"\n 	Score= "<<_blue.score();
			cout<<"\n 	Red Cards= "<<_blue.red_cards();
			cout<<"\n 	Yellow Cards= "<<_blue.yellow_cards();
			cout<<"\n 	Timeouts= "<<_blue.timeouts();
			cout<<"\n 	Goalie= "<<_blue.goalie();

			

				

			

			
			
			chatter_pub.publish(msg);
		}
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
