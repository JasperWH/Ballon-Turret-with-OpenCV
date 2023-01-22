
void fire(){

bool valid_input = false;
do {
    std::cout << "Would you like to fire? [Y/N]: ";
    char choice;
    std::cin >> choice;

    switch (choice){
        case 'Y':

            //Fire
            //--------------------------------------------------------------
            gpioSetMode(3, PI_OUTPUT);          // Motor control signal
            gpioSetMode(2, PI_INPUT);           // Fire feedback - input pins not 5v tolerant - need to connect to 3.3v pin
            gpioSetPullUpDown(2, PI_PUD_UP);    // Set pin as pull up

            while(gpioRead(2) == PI_LOW){
                gpioWrite(3, PI_HIGH);          // Turn motor (causing it to fire) unitil fire feedback to trigger end of firing
            }

            std::cout << "ENGAGED\n";
            gpioWrite(3, PI_LOW);               // Turn motor off

            valid_input = true;                 // Break loop if valid input
            break;

            //Abort
            //--------------------------------------------------------------
        case 'N':
            std::cout << "Engagment aborted\n";
            valid_input = true;                 // Break loop if valid input
            break;

            //Invalid Input
            //--------------------------------------------------------------
        default:
            std::cerr << "That input is invalid.  Please try again.\n";
    }
} while (valid_input == false);

}
