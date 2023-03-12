PROGRAM CITY

IMPLICIT NONE

INTEGER :: i,count
INTEGER :: ios
CHARACTER(20) :: filename
CHARACTER(20), ALLOCATABLE, DIMENSION(:) :: city_list

PRINT *, "Enter filename"
READ *, filename

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Open the file and read number of cities
OPEN(UNIT=9, FILE=filename, FORM="FORMATTED", ACTION="READ",&
                                     STATUS="OLD",IOSTAT=ios)
IF(ios /= 0) THEN
    PRINT *, "Could not open file ",TRIM(filename)," Error code: ", ios
    STOP
END IF

READ (UNIT=9, FMT=100) count
PRINT *, "Number of cities: ",count

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Allocate memory for all needed arrays
ALLOCATE(city_list(1:count),STAT=ios)
IF(ios /= 0) THEN
    PRINT *, "ERROR, could not allocate memory."
    STOP
END IF

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Fill in array from data file
DO i=1, count
    READ (UNIT=9, FMT=200) city_list(i)
END DO

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!Print array 
PRINT *
DO i=1, count
    PRINT *, TRIM(city_list(i)) 
END DO

100 FORMAT (I6)
200 FORMAT (A)

END PROGRAM CITY
