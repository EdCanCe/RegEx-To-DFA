SELECT P.titulo, P.año
FROM Pelicula P
GROUP BY P.titulo
HAVING MIN(P.presupuesto)