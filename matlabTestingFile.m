%% mattsCookedArrayIndexReturn
array = 1234;
index = 5;
arrayLength = floor(log(array)/log(10));
factor = 10^((arrayLength - index + 1));
arrayChunk = floor(array/factor);
indexValue = mod(arrayChunk, 10)

%% mattsCookedArrayAppend
nextValue = 9;
array = array * 10 + nextValue