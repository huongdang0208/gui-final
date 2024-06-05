
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <gui_guider.h>
#include <pthread.h>

void* sensor_thread(void* arg) {
    lv_ui *ui = (lv_ui*)arg;
    
    // Create I2C bus
    int file;
    char *bus = "/dev/i2c-1";
    if ((file = open(bus, O_RDWR)) < 0) {
        printf("Failed to open the bus. \n");
        return NULL;
    }

    // Get I2C device, SHT30 I2C address is 0x44(68)
    if (ioctl(file, I2C_SLAVE, 0x44) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        close(file);
        return NULL;
    }

    while (1) {
        // Send measurement command(0x2C)
        // High repeatability measurement(0x06)
        char config[2] = {0};
        config[0] = 0x2C;
        config[1] = 0x06;
        if (write(file, config, 2) != 2) {
            printf("Failed to write to the i2c bus.\n");
            close(file);
            return NULL;
        }
        sleep(1);

        // Read 6 bytes of data
        // Temp msb, Temp lsb, Temp CRC, Humidity msb, Humidity lsb, Humidity CRC
        char data[6] = {0};
        if (read(file, data, 6) != 6) {
            printf("Failed to read from the i2c bus.\n");
            close(file);
            return NULL;
        } else {
            // Convert the data
            int temp = (data[0] * 256 + data[1]);
            float cTemp = -45 + (175 * temp / 65535.0);
            float humidity = 100 * (data[3] * 256 + data[4]) / 65535.0;

            // Update LVGL labels
            char strTemp[32];
            snprintf(strTemp, sizeof(strTemp), "%.2f °C", cTemp);

            char strHumd[32];
            snprintf(strHumd, sizeof(strHumd), "%.2f %%RH", humidity);

            // Ensure LVGL calls are made from the LVGL thread
            lv_label_set_text(ui->screen_label_49, strTemp);
            lv_label_set_text(ui->screen_label_50, strHumd);

            // Trigger LVGL task handler to refresh the display
            lv_task_handler();
        }
        // Sleep for 5 minutes (300 seconds)
        sleep(300);
    }

    // Close the I2C bus
    close(file);
    return NULL;
}