% Place your solution here
color(green).
color(blue).
color(white).

shoes(natasha, green).
same_color(Color,Color):-
    color(Color).
green_white(Color, OtherColor):-
    same_color(Color, green),
    same_color(OtherColor, white).

green_white(OtherColor, Color):-
    same_color(Color, green),
    same_color(OtherColor, white).


mytask(AnShoes, AnDress, NatShoes, NatDress, WalShoes, WalDress):-
    color(AnShoes),
    color(NatShoes),
    color(WalShoes),
    color(AnDress),
    color(NatDress),
    color(WalDress),
    permutation([green,blue, white], [AnShoes, NatShoes, WalShoes]),
    permutation([green,blue, white], [AnDress, NatDress, WalDress]),
    shoes(natasha, green),
    not(same_color(NatShoes, NatDress)),
    not(same_color(WalShoes, WalDress)),
    not(same_color(WalShoes, white)),
    not(same_color(WalDress, white)),
    not(green_white(AnShoes, AnDress)),
    not(green_white(NatShoes, NatDress)),
    not(green_white(WalShoes, WalDress)).
