# Benchmarks

You can run benchmarks with `make benchmark`.

## Results

### Naive implementation

By applying the N by N kernel to each pixel, we can blur an image in
`O(height * width * n^2)`.

![Naive](./screenshots/naive.png)

### 2 Pass implementation

For some 2D convolutions, we can break it down into two 1D convolutions, one in
each direction. (not so obvious, proof is required to show they are equivalent).

In this case, we only need to apply a 1D kernel (of size N), once in each
direction, which is `O(height * width * n)`.

![2Pass](./screenshots/two-pass.png)

We get a pretty significant speedup here.

- Box blur: 22.6807s -> 87.8006ms = 22680.7ms / 87.8006ms = 258.3x speedup
- Gaussian blur: 33.6078s -> 1.35429s = 33.6078s / 1.35429s = 24.8x speedup

We got a far better speedup with the box blur because we were able to utilize a
sliding window approach, eliminating the need to apply the full kernel for each
pixel. In this case we only need to add/remove elements at the bounds of our
window. I couldn't find a obvious way to apply the same technique to the
gaussian blur due to the weights applied.

> Behold the power of big O

### Kawase Blur

```console
-------------------------------------------------------------------------------
Box Blur Benchmarks
-------------------------------------------------------------------------------
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
Box Blur - 16 kernel size - 1 Pass             100             1      8.1369 s 
                                        84.9325 ms    84.4229 ms    85.7966 ms 
                                        3.29698 ms    2.08502 ms     5.1916 ms 
                                                                               
Box Blur - 32 kernel size - 1 Pass             100             1     8.54063 s 
                                        88.4005 ms     87.961 ms    88.9297 ms 
                                        2.45179 ms     2.0359 ms     3.0346 ms 
                                                                               
Box Blur - 64 kernel size - 2 Pass             100             1      17.878 s 
                                        173.636 ms    173.111 ms    174.315 ms 
                                        3.02911 ms    2.45472 ms    3.86275 ms 
                                                                               

-------------------------------------------------------------------------------
Guassian Blur Benchmarks
-------------------------------------------------------------------------------
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
Gaussian Blur - 16 kernel size - 5                                             
sigma                                          100             1     1.18072 m 
                                        710.611 ms    707.485 ms    714.307 ms 
                                        17.4752 ms    14.8495 ms    21.3806 ms 
                                                                               
Gaussian Blur - 32 kernel size - 5                                             
sigma                                          100             1     2.37497 m 
                                         1.39684 s     1.39236 s     1.40199 s 
                                        24.5941 ms     20.934 ms    29.4177 ms 
                                                                               
Gaussian Blur - 64 kernel size - 10                                            
sigma                                          100             1     4.66035 m 
                                         2.70669 s       2.701 s     2.71304 s 
                                        30.7035 ms    26.9227 ms    35.9736 ms 
                                                                               

-------------------------------------------------------------------------------
Kawase Blur Benchmarks
-------------------------------------------------------------------------------
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
Kawase Blur - 2 Pass                           100             1     6.21852 s 
                                        62.2796 ms    62.2112 ms    62.4949 ms 
                                        548.937 us    119.725 us    1.18748 ms 
                                                                               
Kawase Blur - 4 Pass                           100             1     12.4118 s 
                                        124.607 ms     124.27 ms    125.569 ms 
                                        2.53924 ms    316.338 us    5.33867 ms 
                                                                               
Kawase Blur - 8 Pass                           100             1     24.8109 s 
                                        250.084 ms    249.711 ms    250.715 ms 
                                        2.42331 ms     1.5915 ms    3.69696 ms 
                                                                               
Kawase Blur - 16 Pass                          100             1     49.8835 s 
                                          498.7 ms    497.965 ms    500.042 ms 
                                        4.91352 ms    3.20676 ms    8.60602 ms 
                                                                               
Kawase Blur - 32 Pass                          100             1     1.65514 m 
                                         1.00128 s     1.00014 s     1.00293 s 
                                        6.92422 ms    4.99508 ms    9.61767 ms 
                                                                               

===============================================================================
test cases: 3 | 3 passed
```

After elminating branching and precomputing all the indexes (since it's just
sampling 4 corners), kawase blur is actually crazy fast, beating out box blur.
It also approximates the guassian blur seemingly better than box blur. It
samples 4 points per pixel, making the complexity `O(height * width * passes)`.
