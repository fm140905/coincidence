/*
 * @Description: Event class for a single event
 * @Author: Ming Fang
 * @Date: 2021-03-26 12:31:48
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-26 16:41:24
 */

#pragma once

#include "getInput.h"

#include <vector>
#include "TFile.h"


struct Event
{
    ULong64_t timeStampHeader = 0;

    // std::vector<Float_t> voltage;
    Double_t timeStampDACFD = 0;
    Float_t height = 0;
    UShort_t heightindex = 0;
    Float_t totalIntegral = 0;
    Float_t energy=0;
    Float_t tailIntegral = 0;
    Bool_t isBad = false;
};

class Channel
{
private:
    /* data */
public:
    std::vector<Event> events;
    // Channel(/* args */);
    // ~Channel();
};

// Channel::Channel(/* args */)
// {
// }

// Channel::~Channel()
// {
// }


class CoincidenceChannel
{
private:
    /* data */
public:
    // CoincidenceChannel(/* args */);
    // ~CoincidenceChannel();
};

// CoincidenceChannel::CoincidenceChannel(/* args */)
// {
// }

// CoincidenceChannel::~CoincidenceChannel()
// {
// }
