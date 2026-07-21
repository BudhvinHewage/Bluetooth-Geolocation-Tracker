### Goals
- Positional Tracking of devices in proximity of bluetooth sensor
- Geolocational tracking of devices and by extension, people
- Determine the possibility of utilizing an existing layout of a room or an area, and a central reciever to map out device locations on an axis, with the eventual addition of multiple recievers to triangulate on all axis the location of devices

### Considerations
- This will be used to track specific devices registered within the system, for privacy concerns and avoiding headaches caused by influx of devices pinging the reciever
- It has become a common standard to have devices rotate their BLE MAC addresses rotate so unless the device is paired with the reciever, it will fail to recognize the same device later down the line

### Setup
- Due to current limitations on affordability and money, the testing would be limited to one, and avoiding room recognition, rather just area recognition in that the prescence of a device indicating the prescence of that individual in the area
    - I should be able to get my hands on three ESP32 chips to test triangulation so it should not be a proble no longer
    - Only issue would be the placement and powering of these chips

### Integrations with Existing Systems
- Home Assistant 
    - Recognition of device in proximity of the house and then trigger the vision system to boot up, thereby saving energy and reducing the degradation of the camera
- Personal Agent (Hermes or something else)
    - Proximity triggers personal agent to send specific information regarding the location that the individual walked to
        - If work, sends a notification containing tasks that the person should immediately address or soon once in the office
        - If home, sends notification containing tasks or chores that the person should do
        - If a specific store, sends a reminder of items to purchase that they need or considered getting

### Existing Alternatives
> They are fully fleshed out however the issue with them is that what it would mean for the learning process, and if ever the support dies out or Home Assistant gets intruded upon
- ESPresense
- Bermuda add-on

### Splitting of Responsibilities
> Could have a sharing of tracking and other responsibilities

### Technology
- Resolvable Random Private Address
- Utilize an IRK whereby it is stored in the resolving list
- I have two ESP32, grab more as well and then try the triangulation process