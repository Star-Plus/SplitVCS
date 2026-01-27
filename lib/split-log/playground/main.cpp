#include "main/Logger.h"

int main() {

    Split::Logger logger(true);

    logger.info("This is an info message.");
    logger.warning("This is a warning message.");
    logger.error("This is an error message.");
    logger.debug("This is a debug message.");

    return 0;
}