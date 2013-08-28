--read the README.md file for a description of the lua hooks and globals--
function eat_snack()
  growsnake()
end

function tick(points)
  for i = 1,points do
    printhash(i+10, 0);
  end
end
