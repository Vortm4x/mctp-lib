// #include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <mctp/core/p_message.h>
#include <mctp/core/packet_queue.h>
#include <mctp/core/base.h>


#define MB (1 << 20)

using CatchChrono = Catch::Benchmark::Chronometer;


TEST_CASE("MCTP tx-queue", "[bench]") {
    mctp_pktq_t *queue = NULL;
    uint8_t *msg_data = NULL;
    size_t msg_len = 0;

    auto disasm_func = 
    [&queue, &msg_data, &msg_len]{
        return mctp_message_disassemble(
            queue, 
            msg_data, 
            msg_len, 
            MCTP_EID_NULL,
            MCTP_EID_NULL,
            0, false
        );
    };

    BENCHMARK_ADVANCED("1-mb message")(CatchChrono chrono) {
        msg_len = 1 * MB;
        queue = mctp_pktq_create();
        msg_data = (uint8_t *)malloc(msg_len);

        chrono.measure(disasm_func);

        free(msg_data);
        mctp_pktq_destroy(queue);
    };

    BENCHMARK_ADVANCED("2-mb message")(CatchChrono chrono) {
        msg_len = 2 * MB;
        queue = mctp_pktq_create();
        msg_data = (uint8_t *)malloc(msg_len);

        chrono.measure(disasm_func);

        free(msg_data);
        mctp_pktq_destroy(queue);
    };

    BENCHMARK_ADVANCED("3-mb message")(CatchChrono chrono) {
        msg_len = 3 * MB;
        queue = mctp_pktq_create();
        msg_data = (uint8_t *)malloc(msg_len);

        chrono.measure(disasm_func);

        free(msg_data);
        mctp_pktq_destroy(queue);
    };

    BENCHMARK_ADVANCED("4-mb message")(CatchChrono chrono) {
        msg_len = 4 * MB;
        queue = mctp_pktq_create();
        msg_data = (uint8_t *)malloc(msg_len);

        chrono.measure(disasm_func);

        free(msg_data);
        mctp_pktq_destroy(queue);
    };

    BENCHMARK_ADVANCED("5-mb message")(CatchChrono chrono) {
        msg_len = 5 * MB;
        queue = mctp_pktq_create();
        msg_data = (uint8_t *)malloc(msg_len);

        chrono.measure(disasm_func);

        free(msg_data);
        mctp_pktq_destroy(queue);
    };

    BENCHMARK_ADVANCED("6-mb message")(CatchChrono chrono) {
        msg_len = 6 * MB;
        queue = mctp_pktq_create();
        msg_data = (uint8_t *)malloc(msg_len);

        chrono.measure(disasm_func);

        free(msg_data);
        mctp_pktq_destroy(queue);
    };

    BENCHMARK_ADVANCED("7-mb message")(CatchChrono chrono) {
        msg_len = 7 * MB;
        queue = mctp_pktq_create();
        msg_data = (uint8_t *)malloc(msg_len);

        chrono.measure(disasm_func);

        free(msg_data);
        mctp_pktq_destroy(queue);
    };

    BENCHMARK_ADVANCED("8-mb message")(CatchChrono chrono) {
        msg_len = 8 * MB;
        queue = mctp_pktq_create();
        msg_data = (uint8_t *)malloc(msg_len);

        chrono.measure(disasm_func);

        free(msg_data);
        mctp_pktq_destroy(queue);
    };
}
