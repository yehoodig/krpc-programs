import krpc
import os
conn = krpc.connect()
vessel = conn.space_center.active_vessel
autopilot = vessel.auto_pilot

flight_info = vessel.flight()
vessthrust = conn.add_stream(getattr, vessel, 'thrust')
altitude = conn.add_stream(getattr, flight_info, 'mean_altitude')
apoapsis = conn.add_stream(getattr, vessel.orbit, 'apoapsis')
currentspeed = conn.add_stream(getattr, vessel.flight(vessel.orbit.body.reference_frame), 'speed')

def staging_handler():
    if len(vessel.parts.engines) == 0:
        return
    if vessthrust() < 1:
        if flight_info.true_air_speed < flight_info.terminal_velocity:
            vessel.control.activate_next_stage()

def steering_handler():
    if currentspeed() < 50:
        return 90
    pitch = max(0, 90*(1 - (apoapsis()-600000)/70000))
    autopilot.target_pitch_and_heading(pitch, 90)
    return pitch

def throttle_handler():
    vessel.control.throttle = (90-abs(flight_info.pitch))/90


vessel.control.sas = bool("True")
vessel.control.activate_next_stage()
lasttime = vessel.met
autopilot.target_pitch_and_heading(90, 90)
autopilot.engage()
autopilot.wait()
while altitude() < 70000:
    staging_handler()
    pitch = steering_handler()
    throttle_handler()
    if lasttime+0.25 < vessel.met:
        os.system("clear")
        print "Altitude: ", altitude()
        print "apoapsis: ", apoapsis()
        print "pitch: ", pitch
        print autopilot.pitch_error
        lasttime = vessel.met

print "past alt loop"
while currentspeed() > 265 or altitude() > 5000:
    autopilot.target_direction = vessel.flight(vessel.surface_reference_frame).retrograde
    print "not safe", '\r',
vessel.control.sas = False 
vessel.control.activate_next_stage()

print "end program"
