-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Getting data from the crime_scene_reports table
SELECT * FROM crime_scene_reports
WHERE YEAR = 2023
AND MONTH = 7
AND STREET = 'Humphrey Street';

/*
ID|YEAR|MONTH|DAY|STREET|DESCRIPTION
254|2023|7|6|Humphrey Street|Shoplifting took place at 04:45. Two people witnessed the event.
295|2023|7|28|Humphrey Street|Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
297|2023|7|28|Humphrey Street|Littering took place at 16:36. No known witnesses.
299|2023|7|30|Humphrey Street|Littering took place at 17:49. Two people witnessed the event.
*/

-- Getting data from the bakery_security_logs table
SELECT id, activity, hour, minute, license_plate FROM bakery_security_logs
WHERE YEAR = 2023
AND MONTH = 7
AND DAY = 28
AND HOUR = 10
AND MINUTE BETWEEN 5 AND 25;

/*
ID|ACTIVITY|HOUR|MINUTELICENSE_PLATE
258|entrance|10|8|R3G7486
259|entrance|10|14|13FNH73
260|exit|10|16|5P2BI95
261|exit|10|18|94KL13X
262|exit|10|18|6P58WS2
263|exit|10|19|4328GD8
264|exit|10|20|G412CB7
265|exit|10|21|L93JTIZ
266|exit|10|23|322W7JE
267|exit|10|23|0NTHK55
*/

-- Getting data from the interviews table
SELECT id, transcript FROM Interviews
WHERE YEAR = 2023
AND MONTH = 7
AND DAY = 28;

/*
ID|TRANSCRIPT
158|“Ah,” said he, “I forgot that I had not seen you for some weeks. It is a little souvenir from the King of Bohemia in return for my assistance in the case of the Irene Adler papers.”
159|“I suppose,” said Holmes, “that when Mr. Windibank came back from France he was very annoyed at your having gone to the ball.”
160|“You had my note?” he asked with a deep harsh voice and a strongly marked German accent. “I told you that I would call.” He looked from one to the other of us, as if uncertain which to address.
161|Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
162|I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
163|As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
191|Our neighboring courthouse has a very annoying rooster that crows loudly at 6am every day. My sons Robert and Patrick took the rooster to a city far, far away, so it may never bother us again. My sons have successfully arrived in Paris.
*/

-- Getting data from atm_transactions table
SELECT id, account_number, amount FROM atm_transactions
WHERE YEAR = 2023
AND MONTH = 7
AND DAY = 28
AND ATM_LOCATION = 'Leggett Street'
AND TRANSACTION_TYPE = 'withdraw';

/*
ID|ACCOUNT_NUMBER|AMOUNT
246|28500762|48
264|28296815|20
266|76054385|60
267|49610011|50
269|16153065|80
288|25506511|20
313|81061156|30
336|26013199|35
*/

-- Getting data from bank_accounts and people tables
SELECT * FROM bank_accounts, people
WHERE people.id = bank_accounts.person_id
AND account_number IN
(
    SELECT account_number FROM atm_transactions
    WHERE YEAR = 2023
    AND MONTH = 7
    AND DAY = 28
    AND ATM_LOCATION = 'Leggett Street'
    AND TRANSACTION_TYPE = 'withdraw'
);

/*
ACCOUNT_NUMBER|PERSON_ID|YEAR_OPENED|ACCOUNT_NUMBER|NAME|PHONE_NUMBER|SOCIAL_SECURITY_NUMBER|DRIVER_LICENSE
49610011|686048|2010|686048|Bruce|(367) 555-5533|5773159633|94KL13X
26013199|514354|2012|514354|Diana|(770) 555-1861|3592750733|322W7JE
16153065|458378|2012|458378|Brooke|(122) 555-4581|4408372428|QX4YZN3
28296815|395717|2014|395717|Kenny|(826) 555-1652|9878712108|30G67EN
25506511|396669|2014|396669|Iman|(829) 555-5269|7049073643|L93JTIZ
28500762|467400|2014|467400|Luca|(389) 555-5198|8496433585|4328GD8
76054385|449774|2015|449774|Taylor|(286) 555-6063|1988161715|1106N58
81061156|438727|2018|438727|Benista|(338) 555-6650|9586786673|8X428L0
*/

-- Checking the bank_accounts and people tables for the license plates
SELECT * FROM bank_accounts, people
WHERE people.id = bank_accounts.person_id
AND account_number IN
(
    SELECT account_number FROM atm_transactions
    WHERE YEAR = 2023
    AND MONTH = 7
    AND DAY = 28
    AND ATM_LOCATION = 'Leggett Street'
    AND TRANSACTION_TYPE = 'withdraw'
)
AND license_plate IN 
(
    SELECT license_plate FROM bakery_security_logs
    WHERE YEAR = 2023
    AND MONTH = 7
    AND DAY = 28
    AND HOUR = 10
    AND MINUTE BETWEEN 5 AND 25
);

/*
ACCOUNT_NUMBER|PERSON_ID|YEAR_OPENED|ACCOUNT_NUMBER|NAME|PHONE_NUMBER|SOCIAL_SECURITY_NUMBER|DRIVER_LICENSE
49610011|686048|2010|686048|Bruce|(367) 555-5533|5773159633|94KL13X
26013199|514354|2012|514354|Diana|(770) 555-1861|3592750733|322W7JE
25506511|396669|2014|396669|Iman|(829) 555-5269|7049073643|L93JTIZ
28500762|467400|2014|467400|Luca|(389) 555-5198|8496433585|4328GD8
*/

-- Getting data from phone_calls table
SELECT * FROM phone_calls
WHERE YEAR = 2023
AND MONTH = 7
AND DAY = 28
AND DURATION <= 60;

/*
ID|PHONE_NUMBER|CALLER_ID|YEAR|MONTH|DAY|DURATION
221|(130) 555-0289|(996) 555-8899|2023|7|28|51
224|(499) 555-9472|(892) 555-8872|2023|7|28|36
233|(367) 555-5533|(375) 555-8161|2023|7|28|45
234|(609) 555-5876|(389) 555-5198|2023|7|28|60
251|(499) 555-9472|(717) 555-1342|2023|7|28|50
254|(286) 555-6063|(676) 555-6554|2023|7|28|43
255|(770) 555-1861|(725) 555-3243|2023|7|28|49
261|(031) 555-6622|(910) 555-3251|2023|7|28|38
279|(826) 555-1652|(066) 555-9701|2023|7|28|55
281|(338) 555-6650|(704) 555-2131|2023|7|28|54
*/

-- Checking the bank_accounts and people tables for the phone numbers
SELECT * FROM bank_accounts, people
WHERE people.id = bank_accounts.person_id
AND account_number IN
(
    SELECT account_number FROM atm_transactions
    WHERE YEAR = 2023
    AND MONTH = 7
    AND DAY = 28
    AND ATM_LOCATION = 'Leggett Street'
    AND TRANSACTION_TYPE = 'withdraw'
)
AND license_plate IN 
(
    SELECT license_plate FROM bakery_security_logs
    WHERE YEAR = 2023
    AND MONTH = 7
    AND DAY = 28
    AND HOUR = 10
    AND MINUTE BETWEEN 5 AND 25
)
AND phone_number IN
(
    SELECT caller FROM phone_calls
    WHERE YEAR = 2023
    AND MONTH = 7
    AND DAY = 28
    AND DURATION <= 60
);

/*
ACCOUNT_NUMBER|PERSON_ID|YEAR_OPENED|ACCOUNT_NUMBER|NAME|PHONE_NUMBER|SOCIAL_SECURITY_NUMBER|DRIVER_LICENSE
49610011|686048|2010|686048|Bruce|(367) 555-5533|5773159633|94KL13X
26013199|514354|2012|514354|Diana|(770) 555-1861|3592750733|322W7JE
*/

-- Getting data from flights, airports FA, airports TA tables
SELECT * from flights, airports FA, airports TA
WHERE flights.year = 2023
AND flights.month = 7
AND flights.day = 29
AND FA.ID = flights.origin_airport_id
AND TA.ID = flights.destination_airport_id
AND TA.CITY != 'Fiftyville'
ORDER BY flights.hour ASC, flights.minute ASC
LIMIT 1;

/*
ID|YEAR|MONTH|DAY|HOUR|MINUTE|AIRLINE|FLIGHT_NUMBER|ORIGIN_AIRPORT_ID|DESTINATION_AIRPORT_ID
36|8|4|2023|7|29|8|20|8|CSF|Fiftyville Regional Airport|Fiftyville|4|LGA|LaGuardia Airport|New York City
*/

-- Getting passenger data from passengers table
SELECT * FROM passengers
WHERE flight_id IN
(
    SELECT flights.id from flights, airports FA, airports TA
    WHERE flights.year = 2023
    AND flights.month = 7
    AND flights.day = 29
    AND FA.ID = flights.origin_airport_id
    AND TA.ID = flights.destination_airport_id
    AND TA.CITY != 'Fiftyville'
    ORDER BY flights.hour ASC, flights.minute ASC
    LIMIT 1
);

/*
ID|FLIGHT_ID|FIRST_NAME|LAST_NAME|DOB|PASSPORT_NUMBER|SEAT
36|7214083635|2A
36|1695452385|3B
36|5773159633|4A
36|1540955065|5C
36|8294398571|6C
36|1988161715|6D
36|9878712108|7A
36|8496433585|7B
*/

-- Checking the bank_accounts and people tables for the passport numbers
SELECT * FROM bank_accounts, people
WHERE people.id = bank_accounts.person_id
AND account_number IN
(
    SELECT account_number FROM atm_transactions
    WHERE YEAR = 2023
    AND MONTH = 7
    AND DAY = 28
    AND ATM_LOCATION = 'Leggett Street'
    AND TRANSACTION_TYPE = 'withdraw'
)
AND license_plate IN 
(
    SELECT license_plate FROM bakery_security_logs
    WHERE YEAR = 2023
    AND MONTH = 7
    AND DAY = 28
    AND HOUR = 10
    AND MINUTE BETWEEN 5 AND 25
)
AND phone_number IN
(
    SELECT caller FROM phone_calls
    WHERE YEAR = 2023
    AND MONTH = 7
    AND DAY = 28
    AND DURATION <= 60
)
AND passport_number IN
(
    SELECT passport_number FROM passengers
    WHERE flight_id IN
    (
        SELECT flights.id from flights, airports FA, airports TA
        WHERE flights.year = 2023
        AND flights.month = 7
        AND flights.day = 29
        AND FA.ID = flights.origin_airport_id
        AND TA.ID = flights.destination_airport_id
        AND TA.CITY != 'Fiftyville'
        ORDER BY flights.hour ASC, flights.minute ASC
        LIMIT 1
    )
);

/*
ACCOUNT_NUMBER|PERSON_ID|YEAR_OPENED|ACCOUNT_NUMBER|NAME|PHONE_NUMBER|SOCIAL_SECURITY_NUMBER|DRIVER_LICENSE
49610011|686048|2010|686048|Bruce|(367) 555-5533|5773159633|94KL13X
*/

-- Getting the accomplice's data from the people table
WITH THIEF AS 
(
   SELECT * FROM bank_accounts, people
    WHERE people.id = bank_accounts.person_id
    AND account_number IN
    (
        SELECT account_number FROM atm_transactions
        WHERE YEAR = 2023
        AND MONTH = 7
        AND DAY = 28
        AND ATM_LOCATION = 'Leggett Street'
        AND TRANSACTION_TYPE = 'withdraw'
    )
    AND license_plate IN 
    (
        SELECT license_plate FROM bakery_security_logs
        WHERE YEAR = 2023
        AND MONTH = 7
        AND DAY = 28
        AND HOUR = 10
        AND MINUTE BETWEEN 5 AND 25
    )
    AND phone_number IN
    (
        SELECT caller FROM phone_calls
        WHERE YEAR = 2023
        AND MONTH = 7
        AND DAY = 28
        AND DURATION <= 60
    )
    AND passport_number IN
    (
        SELECT passport_number FROM passengers
        WHERE flight_id IN
        (
            SELECT flights.id from flights, airports FA, airports TA
            WHERE flights.year = 2023
            AND flights.month = 7
            AND flights.day = 29
            AND FA.ID = flights.origin_airport_id
            AND TA.ID = flights.destination_airport_id
            AND TA.CITY != 'Fiftyville'
            ORDER BY flights.hour ASC, flights.minute ASC
            LIMIT 1
        )
    ) 
)
SELECT people.* from phone_calls, THIEF, people
WHERE phone_calls.year = 2023
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration <= 60
AND phone_calls.caller = THIEF.phone_number
AND people.phone_number = phone_calls.receiver;

/*
ID|NAME|PHONE_NUMBER|SOCIAL_SECURITY_NUMBER|DRIVER_LICENSE
864400|Robin|(375) 555-8161||4V16VO0
*/
