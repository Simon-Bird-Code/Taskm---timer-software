#ifndef MY_TIME_H_INCLUDED
#define MY_TIME_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <vector>

using namespace std;

class softwareTimer
{
    public:
    struct stored_times  { int my_usec , my_sec , my_min , my_hour , my_day , my_week , my_year = 0; }st,pt;

    int min_in_sec = 60;  int hour_in_sec = min_in_sec * 60; int day_in_sec = hour_in_sec * 24;
    int week_in_sec = day_in_sec * 7;    long int year_in_sec = day_in_sec * 365;


///=================================
    struct full_time { long int sec , usec;};

    struct t_event { full_time ts; int event_type;};

    vector <full_time> time_segments_start;
    vector <full_time> time_segments_stop;
    vector <full_time> time_segments_diff;

    full_time starting_time , stopping_time , recorded_time , total_running , offset_time, iniz_time , static_time;

    long int t_event_count=0;
    vector <t_event> event_ticker;
///=================================
    int segment_count=0;

    bool running;
    bool Running;
    void clear_stored_times ( void );

    full_time add_full_times ( full_time time_1 , full_time time_2 );

    softwareTimer();    /// iniz all timer instance

    full_time sub_full_times ( full_time time_1 , full_time time_2 );
    void add_time_event ( int event_type );
    void action_start( void );
    void action_stop();


    void start_timer_instance(void);
    void pause_timer_instance(void);
    void restart_timer_instance(void);
    void stop_timer_instance(void);

    string display_current_time(void);

    long int getSecTimes(void);
    long int getUsecTimes(void);
    full_time getFullTime(void);

    full_time getDifference( full_time time_1 , full_time time_2 );

    long int getTotalRunningTime( void );

    void calc_current_running (long int );
    void add_times ( void );

    string get_time_string ( void );
    string get_this_segment_time(void);
    string get_total_running_time(void);

    void test ( full_time full_time_in ){}
};

/// iniz - clear and get current times
softwareTimer::softwareTimer()
{
    clear_stored_times();
    iniz_time.sec=getSecTimes();
    iniz_time.usec=getUsecTimes();
}


/// add and subtract usec and sec, return
softwareTimer::full_time softwareTimer::add_full_times ( full_time time_1 , full_time time_2 )
{
    full_time ret;
    ret.sec= time_1.sec+time_2.sec;
    ret.usec=time_1.usec+time_2.usec;
    return ret;
}

softwareTimer::full_time softwareTimer::sub_full_times ( full_time time_1 , full_time time_2 )
{
    full_time ret;
    ret.sec= time_1.sec-time_2.sec;

    ret.usec=time_1.usec-time_2.usec;
    if (ret.usec<0)
        {
        ret.sec=ret.sec-1;ret.usec=1000000+ret.usec;
        }
    return ret;
}

/// load time into ticker subtract on start
void softwareTimer::action_start()
{
    full_time temp = event_ticker.at(t_event_count).ts;
    total_running  = sub_full_times(temp,offset_time);
}

void softwareTimer::action_stop()
{
    full_time temp = event_ticker.at(t_event_count).ts;
}


void softwareTimer::add_time_event ( int event_type )
{
    t_event temp;
    temp.ts=getFullTime(); temp.event_type=event_type; event_ticker.push_back(temp);
    t_event_count++;
    if(event_type==0) /// shutdown
        {} /// exit class need to make a destructor here
    if(event_type==1) /// start
        {Running = true;action_start();}  /// set running and time store
    if(event_type==2) /// pause
        {Running = false;action_stop();}  /// set running and time store
    ///if((event_type>=48)&&(event_type<=96)){t_event_count++;} /// key enrty

}


void softwareTimer::clear_stored_times ( void )
{
    st.my_day=st.my_hour=st.my_min=st.my_sec=st.my_usec=st.my_week=st.my_year=0;
    pt.my_day=pt.my_hour=pt.my_min=pt.my_sec=pt.my_usec=pt.my_week=pt.my_year=0;
}


long int softwareTimer::getSecTimes(void)
{
    long int ret=time(0);    return ret;
}

long int softwareTimer::getUsecTimes(void)
{
    struct timeval instanceTime;     gettimeofday(&instanceTime, NULL);    return instanceTime.tv_usec;
}

softwareTimer::full_time softwareTimer::getFullTime(void)
{
    full_time ret;
    ret.sec=getSecTimes();ret.usec=getUsecTimes();
    return ret;
}


void softwareTimer::start_timer_instance(void)
{ running = true;}

void softwareTimer::pause_timer_instance(void)
{
    stopping_time.sec=getSecTimes();
    stopping_time.usec=getUsecTimes();

    time_segments_stop.push_back(stopping_time);
   // time_segments_diff.push_back(stopping_time-starting_time);
    segment_count++;
    running = false;
}

void softwareTimer::restart_timer_instance(void)
{
    starting_time.sec=getSecTimes();
    starting_time.usec=getUsecTimes();

    time_segments_start.push_back(starting_time);
    //segment_count++;
    running = true;
}

void softwareTimer::stop_timer_instance(void)
{
stopping_time.sec=getSecTimes();
    stopping_time.usec=getUsecTimes();

    time_segments_stop.push_back(stopping_time);
    segment_count++;
    running = false;

}

void softwareTimer::calc_current_running(long int diff)
{
        bool loop_running=true;    bool calc_done;
        while(loop_running)
            {
                if(loop_running==true){ calc_done=false;}
                if((diff>=year_in_sec) && (calc_done==false)){diff=diff-year_in_sec; st.my_year++;calc_done=true;}
                if((diff>=week_in_sec) && (calc_done==false)){diff=diff-week_in_sec; st.my_week++;calc_done=true;}
                if((diff>=day_in_sec) && (calc_done==false)){diff=diff-day_in_sec; st.my_day++;calc_done=true;}
                if((diff>=hour_in_sec) && (calc_done==false)){diff=diff-hour_in_sec; st.my_hour++;calc_done=true;}
                if((diff>=min_in_sec) && (calc_done==false)){diff=diff-min_in_sec;st.my_min++;calc_done=true;}
                if((diff<min_in_sec) && (calc_done==false)){st.my_sec=diff;calc_done=true;loop_running=false;}
            }
            st.my_usec=getUsecTimes();
}

void softwareTimer::add_times ( void )
{
    st.my_year=st.my_year+pt.my_year;
    st.my_week=st.my_week+pt.my_week;
    st.my_day=st.my_day+pt.my_day;
    st.my_hour=st.my_hour+pt.my_hour;
    st.my_min=st.my_min+pt.my_min;
    st.my_sec=st.my_sec+pt.my_sec;
    st.my_usec=st.my_usec+pt.my_usec;
}


softwareTimer::full_time softwareTimer::getDifference( softwareTimer::full_time time_1 , softwareTimer::full_time time_2 )
{
    full_time ret;
    ret.sec=time_1.sec-time_2.sec;       ret.usec=time_1.usec-time_2.usec;
    return ret;
}

long int softwareTimer::getTotalRunningTime( void )
{
    full_time diff;
    diff.sec=0; diff.usec=0;
    if((int)(time_segments_start.size())!=(int)(time_segments_stop.size()))
    {
        for(int i=0;i<segment_count;i++)
            {
                if(i==(segment_count))
                    {
                        diff.sec=diff.sec+(getSecTimes()-time_segments_start.at(i).sec);
                    }
                else
                    {
                        diff.sec=diff.sec+(time_segments_stop.at(i).sec-time_segments_start.at(i).sec);
                    }
            }
    }
    else
    {
        for(int i=0;i<segment_count;i++)
            {
                diff.sec=diff.sec+(time_segments_stop.at(i).sec-time_segments_start.at(i).sec);
            }
    //getSecTimes()-time_segments.at(0).sec
    //getUsecTimes()-time_segments.at(0)usec
    }

    return diff.sec;//100;
}

string softwareTimer::get_this_segment_time(void)
{
    clear_stored_times();
    if((int)(time_segments_stop.size())!=(int)(time_segments_start.size()))
        {
            calc_current_running( getSecTimes()-time_segments_start.at(segment_count).sec);
            return get_time_string ();
        }
    else
        {
            full_time diff;
            diff.sec=time_segments_stop.at(segment_count-1).sec-time_segments_start.at(segment_count-1).sec;
            calc_current_running(diff.sec);
            return get_time_string ();
        }
}

string softwareTimer::get_total_running_time(void)
{
    /// ////////////////////////////////////////////
    //printf("here");
    /// if compleate segment
    if((int)(time_segments_stop.size())==(int)(time_segments_start.size()))
        {
            //add_times(); put st and pt togethenr??
            calc_current_running(time_segments_stop.at(segment_count).sec - time_segments_start.at(segment_count).sec );
        }
    /// else use running time
    else{
            calc_current_running( getSecTimes()-time_segments_start.at(segment_count).sec);
        }
    return get_time_string ();
    /// ///////////////////////////////////////////
}

string softwareTimer::get_time_string ( void )
{
  //  add_times();
    stringstream ret;
    if (st.my_year<10){ ret << "0" << st.my_year << "\\"; }    else { ret << st.my_year << "\\"; }
    if (st.my_week<10){ ret << "0" << st.my_week << "\\"; }    else { ret << st.my_week << "\\"; }

    ret << st.my_day << " - ";

    if (st.my_hour<10){ ret << "0" << st.my_hour << ":";}    else { ret << st.my_hour << ":"; }
    if (st.my_min<10) { ret << "0" << st.my_min << ":"; }    else { ret << st.my_min << ":"; }
    if (st.my_sec<10) { ret << "0" << st.my_sec << ":"; }    else { ret << st.my_sec << "."; }

    ret << st.my_usec;
    return ret.str().c_str();
}


string softwareTimer::display_current_time(void)
{
    string ret;      return ret;
}