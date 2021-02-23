#include "getTimeStamp.h"


//const UInt_t M = 8;

Int_t getTimeStamp(const Parameters &setting, std::vector< Event > &events)
{
    // std::cout << "Start calculating time stamp using DIACFD..." << std::endl;

    if (!setting.TimeStamp)
    {
        std::cout << "You choose not to calculate the time stamp." << std::endl;
        return 0;
    }
    //std::vector<Float_t> t1;
    //std::vector<Float_t> t2;
/*
    // calculate time stamp using DCFD

    std::cout << "Start calculating time stamp using DCFD..." << std::endl;
    Float_t f = 0.3; // optimized factor f for stilbene = 0.3

    Int_t check = 0;
    Float_t heightTemp(0);
    for(int i=0; i < events.size(); i++)
    {
        heightTemp = f * events[i].height;
        for(int j=0;j<setting.NSamples;j++)
        {
            if(events[i].voltage[j] > heightTemp && events[i].voltage[j-1] <= heightTemp)
            {
                events[i].timeStampDCFD = j - (events[i].voltage[j] - heightTemp)/(events[i].voltage[j] - events[i].voltage[j-1]);
                // t1.push_back(j - (events[i].voltage[j] - heightTemp)/(events[i].voltage[j] - events[i].voltage[j-1]));
                break;
            }
        }
        // check
        // if(!events[i].isBad && !events[i].isPiled && events[i].time_stamp > 90) check += 1;
    }
*/


    // calculate time stamp using DIACFD
    const UInt_t n = setting.NSamples; // setting.NSamples / 2;
    const UInt_t M = setting.Interpolation + 1;
    // UInt_t N = M * n;
    /**************/
    const Int_t T = 30;
    const Int_t L = 6;
    const Int_t windowSize = 2 * setting.Pre; // region where time stamp locates
    const Int_t N = windowSize * M;
    /*************/
    const UInt_t delsampleT = setting.Delt; // setting.Delt * 2;
    const float_t delT = (float_t)delsampleT / M;

    const Float_t f = setting.Fraction;     // optimized factor f for stilbene = 0.25
    Int_t delay = setting.TimeDelay / delT;      // calculate time delay in number of samples
    if (delay == 0) {
        delay = 1;
    }
    const Int_t filterSize = setting.FilterWidth; // size of moving average filter = 3
    TSpline3 *spline3 = 0;
    std::vector<double_t> tim(n);
    std::vector<double_t> vol(n);
    float_t newpulse[N];
    float_t delaypulse[N];
    float_t moveaverage[N];
    // Float_t height(0);
    // std::ofstream newpulseFile("output/interpolatedPulse");
    // std::ofstream oldpulseFile("output/originalPulse");
    // Float_t threshold(0);

    //Int_t check(0);

    // Int_t check = 0;
    Float_t temp(0);
    Int_t tempIndex(0);
    Int_t shift(0);
    //shift = 8192;
    Long64_t starttime(0);

    Int_t totalcounts(0);

    Int_t startIndex(0);
    for(int j = 0; j < 100; j++)
    {
        startIndex += events[j].heightindex;
    }
    startIndex = startIndex / 100 - setting.Pre;
    std::vector<Double_t> coefs;
    sincCoefs(M, T, L, coefs);

    for (int k = 0; k < events.size(); k++)
    {
        //starttime = (Long64_t)events[i].timeStampHeader / 2000 * 2.0;
        // height = events[k].height;
        starttime = events[k].timeStampHeader / 2000 * 2; // the trigger time tag
        if (!events[k].isBad) {
            // interpolation
            for(int j = 0; j < n; j++)
            {
                tim[j] = j * delsampleT;
            }
            for(int j = 0; j < n; j++)
            {
                vol[j] = events[k].voltage[j]; // events[i].voltage[2*j];
            }

            // // cubic interpolation
            // if(M > 1)
            // {
            //     spline3 = new TSpline3("Cubic Spline", &tim[0], &vol[0], n, "b2e2", 0, 0);
            //     // Int_t startIndex = events[0].heightindex - setting.Pre;
            //     for(int j = 0; j < N; j++)
            //     {
            //         newpulse[j] = spline3->Eval(delT * (j+startIndex*M));
            //     }
            // }
            // else
            // {
            //     // Int_t startIndex = events[0].heightindex - setting.Pre;
            //     for(int j = 0; j < N; j++)
            //     {
            //         newpulse[j] = vol[j + startIndex];
            //     }
            // }
            

            // //monotone cubic interpolation
            // if(M > 1)
            // {
            //     std::vector<std::vector<Double_t> > coefs;
            //     FritschCarlson(tim, vol, coefs);
            //     for(int j = 0; j < N; j++)
            //     {
            //         newpulse[j] = monoCubicInter(j*delT, tim, vol, coefs);
            //     }
            // }
            // else
            // {
            //     for(int j = 0; j < N; j++)
            //     {
            //         newpulse[j] = vol[j];
            //     }
            // }

            //tapered sinc interpolation
            if(M > 1)
            {
                Int_t volIndex = startIndex - 1;
                Int_t kk(0);
                for(int j = 0; j < N; j++)
                {
                    kk = j % M;
                    if( kk == 0)
                    {
                        volIndex ++;
                        newpulse[j] = vol[volIndex];
                    }
                    else
                    {
                        newpulse[j] = 0;
                        for(int l = 0; l < L; l++)
                        {
                            newpulse[j] = newpulse[j] + vol[volIndex - l] * coefs[l * M + kk] + vol[volIndex + 1 + l] * coefs[(l + 1)*M - kk];
                        }
                    }
                }
            }
            else
            {
                for(int j = 0; j < N; j++)
                {
                    newpulse[j] = vol[j + startIndex];
                }
            }
            
            // moving average
            for (int j = 0; j <= N - filterSize; j++)
            {
                temp = 0;
                for (int k = 0; k < filterSize; k++)
                {
                    temp += newpulse[j + k];
                }
                moveaverage[j] = temp / filterSize;
            }
            for (int j = N - filterSize + 1; j < N; j++)
            {
                moveaverage[j] = 0;
            }

            // new pulse = fraction multiplied - delayed
            for (int j = 0; j < delay; j++)
            {
                delaypulse[j] = 0;
                newpulse[j] = f * moveaverage[j] - delaypulse[j] + shift;
            }
            for (int j = delay; j < N; j++)
            {
                delaypulse[j] = moveaverage[j - delay];
                newpulse[j] = f * moveaverage[j] - delaypulse[j] + shift;
            }

            
            // approach 1
            // find the maximum of the new pulse
            tempIndex = std::max_element(newpulse, newpulse + N - 1) - newpulse;
            // find the first zero to the right of the maximum
            if (newpulse[tempIndex] < 0)
            {
                events[k].timeStampDACFD = tempIndex * delsampleT + starttime;
                //check++;
            }
            else
            {
                for (int j = tempIndex + 1; j < N; j++)
                {
                    if ((newpulse[j - 1] -shift) >= 0 && (newpulse[j] -shift) < 0)
                    {
                        // events[k].timeStampDACFD = (( j - ( newpulse[j] - shift )/ (newpulse[j] - newpulse[j - 1]))) * delT;
                        // starttime = (Long64_t)events[k].timeStampHeader / 2000;
                        events[k].timeStampDACFD =  ( j - ( newpulse[j] - shift )/ (newpulse[j] - newpulse[j - 1])) * delT;
                        events[k].timeStampDACFD =  events[k].timeStampDACFD + starttime ;
                        break;
                    }
                }
            }
            totalcounts ++;
        }
        else {
            events[k].timeStampDACFD = -1;
        }
        

        // check
        // if(!events[i].isBad && !events[i].isPiled && newpulse[tempIndex] < 0) check += 1;
    }

    //std::cout << check << std::endl;

    std::cout << "Time stamps of " << totalcounts << " good pulses are found." << std::endl;
    return 0;

}
/*
    // Int_t getBipolarPulses(const Parameters &setting, Event  &event, std::vector<std::vector<Float_t>> &bipolarPulses)
    // {
    //     // calculate time stamp using DIACFD
    //     const UInt_t n = setting.NSamples; // setting.NSamples / 2;
    //     const UInt_t M = setting.Interpolation + 1;
    //     const UInt_t N = M * n;
    //     const UInt_t delsampleT = setting.Delt; // setting.Delt * 2;
    //     const float_t delT = (float_t)delsampleT / M;

    //     const Float_t f = setting.Fraction;     // optimized factor f for stilbene = 0.25
    //     Int_t delay = setting.TimeDelay / delT;      // calculate time delay in number of samples
    //     if (delay == 0) {
    //         delay = 1;
    //     }
    //     const Int_t filterSize = setting.FilterWidth; // size of moving average filter = 1

    //     TSpline3 *spline3 = 0;
    //     std::vector<double_t> tim(n);
    //     std::vector<double_t> vol(n);
    //     std::vector<Float_t> newpulse(N);
    //     float_t delaypulse[N];
    //     float_t moveaverage[N];
    //     // Float_t height(0);
    //     // std::ofstream newpulseFile("pulsesHeight");
    //     // Float_t threshold(0);

    //     //Int_t check(0);

    //     // Int_t check = 0;
    //     Float_t temp(0);
    //     // Int_t tempIndex(0);
    //     Int_t shift(0);
    //     //shift = 8192;
    //     //Float_t starttime(0);
    //     //starttime = (Long64_t)events[i].timeStampHeader / 2000 * 2.0;
    //     // height = event.height;

    //     // temp = 0;
    //     // for(int j=0;j<setting.Offset;j++)
    //     // {
    //     //     temp += events[i].samples[j];
    //     // }
    //     // temp /= setting.Offset;
    //     // for(int j=0;j<N;j++)
    //     // {
    //     //     originpulse[j] = events[i].samples[j] - temp;
    //     // }
    //     //newpulseFile << height << '\n';
    //     if (!event.isBad) {
    //         // interpolation
    //         for(int j = 0; j < n; j++)
    //         {
    //             tim[j] = j * delsampleT;
    //         }
    //         for(int j = 0; j < n; j++)
    //         {
    //             vol[j] = event.voltage[j];// event.voltage[2*j];
    //         }
    //         // cubic interpolation
    //         if(M > 1)
    //         {
    //             spline3 = new TSpline3("Cubic Spline", &tim[0], &vol[0], n, "b2e2", 0, 0);
    //             for(int j = 0; j < N; j++)
    //             {
    //                 newpulse[j] = spline3->Eval(delT * j);
    //             }
    //         }
    //         else
    //         {
    //             for(int j = 0; j < N; j++)
    //             {
    //                 newpulse[j] = vol[j];
    //             }
    //         }

    //         // //monotone cubic interpolation
    //         // if(M > 1)
    //         // {
    //         //     std::vector<std::vector<Double_t> > coefs;
    //         //     FritschCarlson(tim, vol, coefs);
    //         //     for(int j = 0; j < N; j++)
    //         //     {
    //         //         newpulse[j] = monoCubicInter(j*delT, tim, vol, coefs);
    //         //     }
    //         // }
    //         // else
    //         // {
    //         //     for(int j = 0; j < N; j++)
    //         //     {
    //         //         newpulse[j] = vol[j];
    //         //     }
    //         // }

        
    //         // moving average
    //         for (int j = 0; j <= N - filterSize; j++)
    //         {
    //             temp = 0;
    //             for (int k = 0; k < filterSize; k++)
    //             {
    //                 temp += newpulse[j + k];
    //             }
    //             moveaverage[j] = temp / filterSize;
    //         }
    //         for (int j = N - filterSize + 1; j < N; j++)
    //         {
    //             moveaverage[j] = 0;
    //         }

    //         // new pulse = fraction multiplied - delayed
    //         for (int j = 0; j < delay; j++)
    //         {
    //             delaypulse[j] = 0;
    //             newpulse[j] = f * moveaverage[j] - delaypulse[j] + shift;
    //         }
    //         for (int j = delay; j < N; j++)
    //         {
    //             delaypulse[j] = moveaverage[j - delay];
    //             newpulse[j] = f * moveaverage[j] - delaypulse[j] + shift;
    //         }

    //         bipolarPulses.push_back(newpulse);



    //         // print the first 100 new pulses
    //         // if(i<200)
    //         // {
    //         //     for(int j=0;j<N;j++)
    //         //     {
    //         //         newpulseFile << newpulse[j] <<"  ";
    //         //     }
    //         //     newpulseFile << std::endl;
    //         // }
    //     }

    //     // std::cout << check << std::endl;
    //     //newpulseFile.close();

    //     return 0;
    // }

    // // implement monotone cubic interpolation
    // int FritschCarlson(const std::vector<double> &xs, const std::vector<double>& ys, std::vector<std::vector<double> >& coefs)
    // {
    //     const int N = xs.size();
    //     std::vector<double> dxs(N-1);
    //     std::vector<double> dys(N-1);
    //     std::vector<double> ms(N-1);
    //     std::vector<double> c1s(N);
    //     std::vector<double> c2s(N-1);
    //     std::vector<double> c3s(N-1);
    //     double m;
    //     double mnext;
    //     double common;
    //     for(int i = 0; i < N - 1; i++)
    //     {
    //         dxs[i] = xs[i+1] - xs[i];
    //         dys[i] = ys[i+1] - ys[i];
    //         ms[i] = dys[i] / dxs[i];
    //     }
    //     c1s[0] = ms[0];
    //     for(int i = 0; i < dxs.size() - 1; i++)
    //     {
    //         m = ms[i];
    //         mnext = ms[i+1];
    //         if (m * mnext <= 0) {
    //             c1s[i+1] = 0;
    //         }
    //         else
    //         {
    //             c1s[i+1] = 3*(dxs[i] + dxs[i+1])/((dxs[i] + 2 * dxs[i+1])/m+(2 * dxs[i] + dxs[i+1])/mnext);
    //         }
    //     }
    //     c1s[N-1] = ms[N-2];
    //     for(int i = 0; i < c1s.size() - 1; i++)
    //     {
    //         m = ms[i];
    //         common = c1s[i] + c1s[i+1] - 2 * m;
    //         c2s[i] = (m - c1s[i] - common) / dxs[i];
    //         c3s[i] = common / dxs[i] / dxs[i];
    //     }
    //     coefs.push_back(c1s);
    //     coefs.push_back(c2s);
    //     coefs.push_back(c3s);
    //     return 0;
    // }

    // double monoCubicInter(double x, const std::vector<double> &xs, const std::vector<double>& ys, const std::vector<std::vector<double> >& coefs)
    // {
    //     const int N = xs.size();
    //     double y(0);
    //     if (x == xs[N-1]) {
    //             y = ys[N-1];
    //             // std::cout << "x = " << x << ", y = " << y << std::endl;
    //             // return 0;
    //             return y;
    //         }
    //         int low = 0; 
    //         int mid = 0;
    //         int high = N -2;
    //         double xHere = -1;
    //         while(low <= high){
    //             mid = (low + high) / 2;
    //             xHere = xs[mid];
    //             if (xHere < x) {
    //                 low = mid + 1;
    //             }
    //             else if (xHere > x)
    //             {
    //                 high = mid - 1;
    //             }
    //             else
    //             {
    //                 y = ys[mid];
    //                 // std::cout << "x = " << x << ", y = " << y << std::endl;
    //                 // return 0;
    //                 return y;
    //             }
    //         }
    //         int index = 0;
    //         if (high > index) {
    //             index = high;
    //         }
    //         double diff = x - xs[index];
    //         y = ys[index] + coefs[0][index] * diff + coefs[1][index] * diff * diff + coefs[2][index] * diff * diff * diff;
    //         // std::cout << "x = " << x << ", y = " << y << std::endl;
    //         // return 0;
    //         return y;
    // }
*/
Int_t sincCoefs(const Int_t N, const Int_t T, const Int_t L, std::vector<double>& coefs)
{
    Double_t PI(3.14159265358979323846);
    Double_t temp(0);
    double_t phi(0);
    coefs.push_back(1);
    for(int j = 1;j< L * N;j++)
    {
        phi = PI * Double_t(j) / N;
        temp = Double_t(j) / Double_t(T);
        temp = sin(phi) / phi * exp(-temp * temp);
        coefs.push_back(temp);
    }
}