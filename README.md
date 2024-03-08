# AUTO SCHEDULING
 Automation of event scheduling for college fests

 # TL;DR
Data is passed to the code as a csv generated through the form mentioned below. The code assigns venues based on availability to events based on first come first serve (whoever fills the form first) for now, but can be changed to based on some key like participation (by sorting the relevant array before passing it to the function). The results are stored in ```output.txt```

# DATA PARSING

 The data is collected through a google form having fields similar to: https://forms.gle/LRqcLZ2bfEmgCK168 and then downloaded as a csv file, which is then passed to the program as a command line argument. Pay special attention to the data input formats mentioned in the form. There is a particular way of entering prefered times (in 24h format without colons, and without leading 0s)

 # USAGE

  After modifying the code to create a suitable array of Venue object pointers, run the following command:
  
 ```g++ autoscheduling.cpp data.csv```

 assuming that data is stored in data.csv. Make sure to give the correct file path if it is present in some other directory. The output is appended to output.txt. According to specific usecase it might be desirable to rewrite the output.txt file instead of appending, in that case change ```line 81``` of ```autoscheduling.cpp``` .

 More information about the schedule can be extracted by writing an appropriate print_schedule function over the list of events. Alternatively, the availability array of ```class Venue``` can be change to a map of int and Event, and then an appropriate print function could be written. If you make this changes, please make a pull request and i'll be happy to merge.

 # FURTHER WORK

  Three main problems right now:
1) The events need to start and end on the same day, which is a strong condition in real life. For example, an event beginning at day1,2330 and ending on day2,0200 will not be accomodated in the current code
2) The current policy of preference accomodation works like this: if first venue preference is not available at first time preference, then instead of checking subsequent venue preference the first venue preference is checked at the subsequent time preferences. This may not be desirable in all scenarios. One way to overcome this might be to include a bool in the Class definition of Event, and ask the event organisers if they care about time or venue more, then modify the assignment function through 2 control flows based on the bool flag. Too much work for now, again happy to merge if you open a pull request.
3) The current priority of events is on first come first serve. It might be desirable to sort the vector of Event objects based on participation or some other key. Again, happy to merge if you open a pull request.
4) The implementation is just really inefficient and ugly. Changes to be made: Use a priority queue to store events, based on key as participation. Use an unordered map between time_slots (ints) and availabilities (bools) instead of avaialability array. Use an ordered set for time and venue priorities. 

   Ok bye, ly <3
