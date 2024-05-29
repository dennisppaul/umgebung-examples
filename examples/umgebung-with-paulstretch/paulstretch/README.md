# paulstretch

forked from https://github.com/paulnasca/paulstretch_cpp

this is a simplified version of paulstretch. in `main.cpp` it is illustrated how to use the `‌ProcessedStretch` class to continously feed blocks of sample data to the algorithm to produce the stretched results. the 4 steps to achieve looks approximately like this:

1. get required samples: `‌stretch.get_required_samples()`
2. fill input buffer with number of required samples: `‌stretch.get_input_buffer()`
3. process input buffer: `‌stretch.process_segment(output_buffer)`
4. use output buffer samples stored in `output_buffer`

note, that in this version of `paulstretch` all *extras* have been removed.
