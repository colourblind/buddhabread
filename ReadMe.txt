Buddhabread is an implementation of Melinda Green's Buddhabrot algorithm. 
Buddhabrot is an alternative way of rendering the Mandelbrot set: using the 
paths of the escaping points rather than the number of iterations until 
they escape.

buddhabread [size] [maxIterations] [samplesPerSide]

[size]
this is the square size of the resulting image

[maxIterations]
the number of iterations to run before deciding a value point is bounded 
or not. A higher value will add more detail. Anything over 200000 seems 
to have very little impact.

[samplesPerSide]
how many samples to take per edge. I like 2400, but if you go for a high-res 
image you will need to use a larger size or the result will be grainy
