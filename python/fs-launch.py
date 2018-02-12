import krpc
conn = krpc.connect()
vessel = conn.space_center.active_vessel

flight_info = vessel.flight()
vessthrust = conn.add_stream(getattr, vessel, 'thrust')
altitude = conn.add_stream(getattr, flight_info, 'mean_altitude')
currentspeed = conn.add_stream(getattr, vessel.flight(vessel.orbit.body.reference_frame), 'speed')

vessel.control.sas = bool("True")
vessel.control.activate_next_stage()
while vessthrust() > 1:
    print altitude(), '\r',

while currentspeed() > flight_info.terminal_velocity:
    print currentspeed(), '\r',

vessel.control.activate_next_stage()
print("\nnext stage \n")
while currentspeed() > 265 or altitude() > 5000:
    print "this is not speed", '\r',
vessel.control.activate_next_stage()
vessel.control.sas = bool("False") 
