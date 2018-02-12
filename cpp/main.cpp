#include <iostream>
#include <algorithm>
#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include <krpc/services/space_center.hpp>

using namespace std;

void staging_handler(krpc::services::SpaceCenter::Vessel vessel) {
  //python
  //if len(vessel.parts.engines) == 0:
  //    return
  //if vessthrust() < 1:
  //    if flight_info.true_air_speed < flight_info.terminal_velocity:
  //        vessel.control.activate_next_stage()
}
float steering_handler(double currentspeed, double apoapsis) {
   if(currentspeed < 50)
      return 90.0;
   float pitch = max(0.0, 90*(1 - (apoapsis-600000)/70000));
   return pitch;
}
void throttle_handler(float pitch) {
  // vessel.control.throttle = (90-abs(flight_info.pitch))/90
}

int main() {
   auto conn = krpc::connect();
   krpc::services::KRPC krpc(&conn);

   krpc::services::SpaceCenter sc(&conn);
   auto vessel = sc.active_vessel();
   std::cout << "Connected to kRPC server version " << krpc.get_status().version() << std::endl;

   auto autopilot = vessel.auto_pilot();
   //flight_info = vessel.flight()
   //vessthrust = conn.add_stream(getattr, vessel, 'thrust')
   auto vessthrust = vessel.thrust_stream();
   //altitude = conn.add_stream(getattr, flight_info, 'mean_altitude')
   auto altitude = vessel.flight().mean_altitude_stream();
   //apoapsis = conn.add_stream(getattr, vessel.orbit, 'apoapsis')
   auto apoapsis = vessel.orbit().apoapsis_stream();
   //currentspeed = conn.add_stream(getattr, vessel.flight(vessel.orbit.body.reference_frame), 'speed')
   auto currentspeed = vessel.flight(vessel.orbit().body().reference_frame()).speed_stream();

   // Python code to be ported
   vessel.control().set_sas(true);
   vessel.control().activate_next_stage();
   autopilot.target_pitch_and_heading(90, 90);
   autopilot.engage();
   autopilot.wait();
	
   double lasttime = vessel.met();
   while(altitude() < 70000) {
      staging_handler(vessel);
      float pitch = steering_handler(currentspeed(), apoapsis());
      autopilot.target_pitch_and_heading(pitch, 90);
      throttle_handler(vessel.flight().pitch());
      if(lasttime+0.25 < vessel.met()){
	 system("clear");
	 cout << "Altitude: "  << altitude() << std::endl
	      << "Apoapsis: "  << apoapsis() << std::endl
	      << "Pitch: "     << pitch      << std::endl
	      << "Pitch Err: " << autopilot.pitch_error() << std::endl;
	 lasttime = vessel.met();
   //        print "Altitude: ", altitude()
   //        print "apoapsis: ", apoapsis()
   //        print "pitch: ", pitch
   //        print autopilot.pitch_error
   //        lasttime = vessel.met
     }
   }
   //
   //print "past alt loop"
   //while currentspeed() > 265 or altitude() > 5000:
   //    autopilot.target_direction = vessel.flight(vessel.surface_reference_frame).retrograde
   //    print "not safe", '\r',
   //vessel.control.sas = False 
   //vessel.control.activate_next_stage()
   //
   //print "end program"
   cout << "end program " << std::endl;

}

