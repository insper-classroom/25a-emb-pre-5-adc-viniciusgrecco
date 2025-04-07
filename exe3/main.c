    #include <FreeRTOS.h>
    #include <task.h>
    #include <semphr.h>
    #include <queue.h>

    #include "pico/stdlib.h"
    #include <stdio.h>

    #include "data.h"
    QueueHandle_t xQueueData;
    int dataList[5];
    int dataIndex = 0;
    int sum = 0;

    // não mexer! Alimenta a fila com os dados do sinal
    void data_task(void *p) {
        vTaskDelay(pdMS_TO_TICKS(400));

        int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
        for (int i = 0; i < data_len; i++) {
            xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
        }

        while (true) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    void process_task(void *p) {
        int data = 0;

        while (true) {
            if (xQueueReceive(xQueueData, &data, pdMS_TO_TICKS(100))) {
                // implementar filtro aqui!
                if (dataIndex < 5){
                    dataList[dataIndex] = data;
                    sum += data;
                    int mediamovel = sum/5;
                    printf("%d\n",mediamovel);
                    dataIndex++;
                } 
                if (dataIndex >= 5){
                    dataList[0] = dataList[1];
                    dataList[1] = dataList[2];
                    dataList[2] = dataList[3];
                    dataList[3] = dataList[4];
                    dataList[4] = data;
                    int mediamovel = (dataList[0] + dataList[1] + dataList[2] + dataList[3] + dataList[4])/5;
                    printf("%d\n",mediamovel);

                }





                // deixar esse delay!
                vTaskDelay(pdMS_TO_TICKS(50));
            }
        }
    }

    int main() {
        stdio_init_all();

        xQueueData = xQueueCreate(64, sizeof(int));

        xTaskCreate(data_task, "Data task ", 4096, NULL, 1, NULL);
        xTaskCreate(process_task, "Process task", 4096, NULL, 1, NULL);

        vTaskStartScheduler();

        while (true)
            ;
    }
