#include "usb.h"
#include "max3421e/max3421e_constants.h"
#include "max3421e/max3421e_usb.h"

void usb_initEndPoint(usb_endpoint * endpoint, uint8_t address)
{
	endpoint->address = address;
	endpoint->sendToggle = bmSNDTOG0;
	endpoint->receiveToggle = bmRCVTOG0;
}

/**
 * Print USB device information.
 */
int usb_printDeviceInfo(usb_device * device)
{
	int rcode;
    char buf[128];

    // Read the device descriptor
	usb_deviceDescriptor deviceDescriptor;
    rcode = usb_getDeviceDescriptor(device, &deviceDescriptor);
    if (rcode) return rcode;

    avr_serialPrintf("Vendor ID: %x\n", deviceDescriptor.idVendor);
    avr_serialPrintf("Product ID: %x\n", deviceDescriptor.idProduct);
    avr_serialPrintf("Configuration count: %d\n", deviceDescriptor.bNumConfigurations);
    avr_serialPrintf("Device class: %d\n", deviceDescriptor.bDeviceClass);
    avr_serialPrintf("Device subclass: %d\n", deviceDescriptor.bDeviceSubClass);
    avr_serialPrintf("Device protocol: %d\n", deviceDescriptor.bDeviceProtocol);

    usb_getString(device, deviceDescriptor.iManufacturer, device->firstStringLanguage, 128, buf);
    avr_serialPrintf("Manufacturer: %d %s\n", deviceDescriptor.iManufacturer, buf);

    usb_getString(device, deviceDescriptor.iProduct, device->firstStringLanguage, 128, buf);
    avr_serialPrintf("Product: %s\n", buf);

    usb_getString(device, deviceDescriptor.iSerialNumber, device->firstStringLanguage, 128, buf);
    avr_serialPrintf("Serial number: %s\n", buf);

    return 0;
}

int usb_initDevice(usb_device * device, int configuration)
{
	char buf[4];

	avr_serialPrintf("Initialising USB device ... %d %d\n", device->address, configuration);

	uint8_t rcode;

	// Set the configuration for this USB device.
	rcode = usb_setConfiguration(device, configuration);
	if (rcode<0) return rcode;

	// Get the first supported language.
	rcode = usb_getString(device, 0, 0, 4, buf);
	if (rcode<0) return rcode;
    device->firstStringLanguage = (buf[3] << 8) | buf[2];

    return rcode;
}