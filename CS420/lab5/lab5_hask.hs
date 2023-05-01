import System.Random (newStdGen, randomRs)
import Data.Time.Clock (getCurrentTime, diffUTCTime)
import Control.Exception (evaluate)

-- create a sorted array of specified size
inputSortedArray :: Int -> [Int]
inputSortedArray len = [1..len]

-- create a reverse array of specified size
inputReverseArray :: Int -> [Int]
inputReverseArray len = reverse [1..len]

-- create random array of specified size
inputRandomArray :: Int -> IO [Int]
inputRandomArray len = do
    gen <- newStdGen
    return $ take len (randomRs (1, 1000000) gen)

-- applies a quicksort algorithm to the given array
quicksort :: Ord a => [a] -> [a]
quicksort [] = []
quicksort (x:xs) = 
    let smallerSorted = quicksort [a | a <- xs, a <= x]
        biggerSorted = quicksort [a | a <- xs, a > x]
    in smallerSorted ++ [x] ++ biggerSorted

-- applies the mergesort algorithm to the given array
mergesort :: Ord a => [a] -> [a]
mergesort [] = []
mergesort [x] = [x]
mergesort xs =
    let (left, right) = splitAt (length xs `div` 2) xs
    in merge (mergesort left) (mergesort right)

-- merge helper function to mergesort
merge :: Ord a => [a] -> [a] -> [a]
merge [] rs = rs
merge ls [] = ls
merge (l:ls) (r:rs)
    | l <= r    = l : merge ls (r:rs)
    | otherwise = r : merge (l:ls) rs

-- Measure time for a function to run
time :: Ord a => ([a] -> [a]) -> [a] -> IO ([a], Double)
time func x = do
    start <- getCurrentTime
    let sorted = func x
    end <- getCurrentTime
    let timeDiff = realToFrac $ diffUTCTime end start
    res <- evaluate sorted
    return (res, timeDiff)

main :: IO ()
main = do
    let array_size = 10000

    let sortedList = inputSortedArray array_size
    (list, timetaken) <- time quicksort sortedList
    putStrLn $ "Time taken: " ++ show timetaken ++ " seconds"

