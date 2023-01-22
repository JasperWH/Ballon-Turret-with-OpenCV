
void computePID(double* PIDarr, CClock::time_point previousTime){

    // Use the previousTime past to the function and subtract from the current time to get time elipsed
    currentTime = CClock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>( currentTime - previousTime ).count();

    // Save last PID calulated values to variables
    double error = PIDarr[0];
    double previous_CumError = PIDarr[1];
    double previous_LastError = PIDarr[2];

    double cumError = previous_CumError + (error * elapsedTime);   // Compute integral
    double rateError = (error - previous_LastError)/elapsedTime;   // Compute derivative
    double lastError = error;                                      // Remember current error

    double output = kp*error + ki*cumError + kd*rateError;         // PID output

    // have array to return the PID output and errors
    PIDarr[1] = cumError;
    PIDarr[2] = lastError;
    PIDarr[3] = output;

}
