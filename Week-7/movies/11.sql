-- List the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated
SELECT title FROM movies
WHERE id IN
(
    SELECT movie_id FROM stars
    WHERE person_id = (
        SELECT id FROM people
        WHERE name = 'Chadwick Boseman'
    )
)
ORDER BY (SELECT rating FROM ratings WHERE movie_id = id) DESC
LIMIT 5;
