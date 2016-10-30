#include "pebble.h"


#define SCREENWIDTH 140
#define SCREENHEIGHT 168
#define AUTOSCROLLHEIGHT 2000
#define QUOTE_Y_BEGIN 60
#define KEY_DAILY_QUOTE 1

static Window *main_window;
static PropertyAnimation *scroll_animation;
static TextLayer *text_layer_quote;
static TextLayer *time_layer;
static int pixels_to_scroll;

static uint32_t key_daily_quote = 0;
static uint32_t key_idx_daily_quote = 1;

//quotes collection partly taken from https://litemind.com.
static char* QUOTES[] = {
		"I use emotion for the many and reserve reason for the few.,Adolf Hitler",
		"Do not look where you fell, but where you slipped.,African proverb",
		"Logic will get you from A to B. Imagination will take you everywhere.,Albert Einstein",
		"A clever person solves a problem. A wise person avoids it.,Albert Einstein",
		"The only thing that interferes with my learning is my education.,Albert Einstein",
		"Blessed is he who expects nothing, for he shall never be disappointed.,Alexander Pope",
		"It is easier to fight for one's principles than to live up to them.,Alfred Adler",
		"We don't see things as they are. We see things as we are.,Anais Nin",
		"It is better to fail at your own life than to succeed at someone else's.,André Gide",
		"Believe those who are seeking the truth. Doubt those who find it.,André Gide",
		"It is bad luck to be superstitious.,Andrew W. Mathis",
		"What lies in our power to do, lies in our power not to do.,Aristotle",
		"We are what we repeatedly do; excellence, then, is not an act but a habit.,Aristotle",
		"Every nation ridicules other nations, and all are right.,Arthur Schopenhauer",
		"I'd rather live with a good question than a bad answer.,Aryeh Frimer",
		"The Argument from Intimidation is a confession of intellectual impotence.,Ayn Rand",
		"The real problem is not whether machines think but whether men do.,B. F. Skinner",
		"Either write something worth reading or do something worth writing.,Benjamin Franklin",
		"What happens to the hole when the cheese is gone?,Bertolt Brecht",
		"No one gossips about other people's secret virtues.,Bertrand Russell",
		"If at first you don't succeed, find out if the loser gets anything.,Bill Lyon",
		"Careful. We don't want to learn from this.,Bill Watterson",
		"A single question can be more influential than a thousand statements.,Bo Bennett",
		"The less you know, the more you believe.,Bono",
		"When the student is ready the teacher will appear.,Buddhist Proverb",
		"You don't get anything clean without getting something else dirty.,Cecil Baxter",
		"Happiness is the absence of striving for happiness.,Chang-Tzu",
		"One dog barks at something. And a hundred dogs bark at the sound.,Chinese Proverb",
		"I hear: I forget / I see: I remember / I do: I understand,Chinese Proverb ",
		"Inspiration is for amateurs. I just get to work.,Chuck Close",
		"Success is getting what you want. Happiness is wanting what you get.,Dale Carnegie",
		"You can do anything, but not everything.,David Allen",
		"A committee can make a decision that is dumber than any of its members.,David Cobitz",
		"The first step toward success will be the biggest one.,Dennis Waitley",
		"It's a rare person who wants to hear what he doesn't want to hear.,Dick Cavett",
		"If your parents never had children, chances are you won't, either.,Dick Cavett",
		"Procrastination is the art of keeping up with yesterday.,Don Marquis",
		"If you are not criticized, you may not be doing much.,Donald H. Rumsfeld",
		"The art of simplicity is a puzzle of complexity.,Doug Horton",
		"I love deadlines. I like the whooshing sound they make as they fly by.,Douglas Adams",
		"In God we trust; all others bring data.,Dr. Edwards Deming",
		"The best car safety device is a rear-view mirror with a cop in it.,Dudley Moore",
		"I am patient with stupidity but not with those who are proud of it.,Edith Sitwell",
		"If only we'd stop trying to be happy we could have a pretty good time.,Edith Wharton",
		"Talent does what it can; genius does what it must.,Edward Bulwer-Lytton",
		"No one can make you feel inferior without your consent.,Eleanor Roosevelt",
		"The cure for boredom is curiosity. There is no cure for curiosity.,Ellen Parr",
		"I don't know anything about music. In my line you don't have to.,Elvis Presley",
		"If voting changed anything, they'd make it illegal.,Emma Goldman",
		"Courage is grace under pressure.,Ernest Hemingway",
		"Character is what you have left when you've lost everything you can lose.,Evan Esar",
		"Writing about music is like dancing about architecture.,Frank Zappa",
		"You must be the change you wish to see in the world.,Gandhi",
		"All animals are equal but some animals are more equal than others.,George Orwell",
		"Those who cannot remember the past are condemned to repeat it.,George Santayana",
		"A physicist is an atom's way of knowing about atoms.,George Wald",
		"2 is not equal to 3, not even for large values of 2.,Grabel's Law",
		"Those are my principles. If you don't like them I have others.,Groucho Marx",
		"There is no truth. There is only perception.,Gustave Flaubert",
		"Think like a man of action, act like a man of thought.,Henri Bergson",
		"Whether you think you can or think you can't -- you are right.,Henry Ford",
		"Science is organized knowledge. Wisdom is organized life.,Immanuel Kant",
		"If I have seen further it is by standing on the shoulders of giants.,Isaac Newton",
		"Once the game is over, the King and the pawn go back in the same box.,Italian Proverb",
		"The world can only be grasped by action, not by contemplation.,Jacob Bronowski",
		"Dream as if you'll live forever. Live as if you'll die today.,James Dean",
		"A myth is a religion in which no one any longer believes.,James Feibleman",
		"It is better to know some of the questions than all of the answers.,James Thurber",
		"He knows all about art, but he doesn't know what he likes.,James Thurber",
		"Only the mediocre are always at their best.,Jean Giraudoux",
		"A person who trusts no one can't be trusted.,Jerome Blattner",
		"I like work: it fascinates me. I can sit and look at it for hours.,Jerome K. Jerome",
		"Sometimes the road less traveled is less traveled for a reason.,Jerry Seinfeld",
		"If God lived on earth, people would break his windows.,Jewish Proverb",
		"If the phone doesn't ring, it's me.,Jimmy Buffet",
		"We are the people our parents warned us about.,Jimmy Buffett",
		"The only normal people are the ones you don't know very well.,Joe Ancis",
		"It is in self-limitation that a master first shows himself.,Johann Goethe",
		"No wise man ever wished to be younger.,Jonathan Swift",
		"Man has to suffer. When he has no real afflictions, he invents some.,Jose Marti",
		"A single death is a tragedy; a million deaths is a statistic.,Joseph Stalin",
		"Imagination is the one weapon in the war against reality.,Jules de Gaultier",
		"Laziness is nothing more than the habit of resting before you get tired.,Jules Renard",
		"If you want truly to understand something, try to change it.,Kurt Lewin",
		"Those who believe in telekinetics, raise my hand.,Kurt Vonnegut",
		"A journey of a thousand miles begins with a single step.,Lao Tzu",
		"Truth over Harmony.,Larry Bossidy ",
		"Simplicity is the ultimate sophistication.,Leonardo da Vinci",
		"Sometimes I worry about being a success in a mediocre world.,Lily Tomlin",
		"The best way to have a good idea is to have lots of ideas.,Linus Pauling",
		"Love thy neighbour as yourself, but choose your neighbourhood.,Louise Beal",
		"Wisdom is knowing the right path to take… Integrity is taking it.,M. H. McKee",
		"An eye for eye only ends up making the whole world blind.,Gandhi",
		"God has no religion.,Gandhi",
		"Education's purpose is to replace an empty mind with an open one.,Malcolm Forbes",
		"If everything's under control, you're going too slow.,Mario Andretti",
		"Never argue with a fool, onlookers may not be able to tell the difference.,Mark Twain",
		"If you have to eat a frog, don't look at it for too long.,Mark Twain",
		"If you tell the truth you don't have to remember anything.,Mark Twain",
		"I don't necessarily agree with everything I say.,Marshall McLuhan",
		"If God wanted us to fly, He would have given us tickets.,Mel Brooks",
		"I can accept failure, but I can't accept not trying.,Michael Jordan",
		"The more you can dream, the more you can do.,Michael Korda",
		"A man who fears suffering is already suffering from what he fears.,Montaigne",
		"We can do no great things, only small things with great love.,Mother Teresa",
		"Never interrupt your enemy when he is making a mistake.,Napoleon Bonaparte",
		"I like long walks, especially when they are taken by people who annoy me.,Noel Coward",
		"Heroism consists of hanging on one minute longer. ,Norwegian saying",
		"The little I know I owe to my ignorance.,Orville Mars",
		"Always forgive your enemies; nothing annoys them so much.,Oscar Wilde",
		"True friends stab you in the front.,Oscar Wilde",
		"Be yourself; everyone else is already taken.,Oscar Wilde",
		"I am not young enough to know everything.,Oscar Wilde",
		"Truth, in matters of religion, is simply the opinion that has survived.,Oscar Wilde",
		"Every saint has a past, and every sinner has a future.,Oscar Wilde",
		"Laws are like sausages. It's better not to see them being made.,Otto von Bismarck",
		"Action is the foundational key to all success.,Pablo Picasso",
		"Computers are useless. They can only give you answers.,Pablo Picasso",
		"The best way to make your dreams come true is to wake up.,Paul Valery",
		"Write drunk, revise sober.,Poet's Motto",
		"Some things have to be believed to be seen.,Ralph Hodgson",
		"I hate quotations. Tell me what you know.,Ralph Waldo Emerson",
		"You have to know how to accept rejection and reject acceptance.,Ray Bradbury",
		"I quit therapy because my analyst was trying to help me behind my back.,Richard Lewis",
		"One of the keys to happiness is a bad memory.,Rita Mae Brown",
		"An undefined problem has an infinite number of solutions.,Robert A. Humphrey",
		"Understanding is simple. Knowing is complicated.,Robert Fripp",
		"My wife and I were happy for twenty years. Then we met.,Rodney Dangerfield",
		"Abortion is advocated only by persons who have themselves been born.,Ronald Reagan",
		"O Lord, help me to be pure, but not yet.,Saint Augustine",
		"What is written without effort is in general read without pleasure.,Samuel Johnson",
		"An honest politician is one who, when he is bought, will stay bought.,Simon Cameron",
		"An honest politician is one who, when he is bought, will stay bought.,Simon Cameron",
		"The only true wisdom is in knowing you know nothing.,Socrates",
		"The unexamined life is not worth living.,Socrates",
		"Fortune does not change men, it unmasks them.,Suzanne Necker",
		"We don't see the things the way they are. We see things the way WE are.,Talmund",
		"If I only had a little humility, I'd be perfect.,Ted Turner",
		"The way to succeed is to double your error rate.,Thomas J. Watson",
		"Invention is the mother of necessity.,Thorstein Veblen",
		"I have such a high regard for the truth that I use it sparingly.,Timothy Connor",
		"Women who seek to be equal with men lack ambition.,Timothy Leary",
		"A cult is a religion with no political power.,Tom Wolfe",
		"The things you own end up owning you.,Tyler Durden, in Fight Club",
		"Opportunity may knock only once, but temptation leans on the doorbell.,Unknown Author",
		"I wish I was as certain about anything, as he is about everything.,Unknown Author",
		"Management works within the system. Leadership works on the system.,Unknown Author",
		"If you don't know where you're going, you don't know when you're lost.,Unknown Author",
		"Be kind to unkind people - they need it the most.,Unknown Author",
		"It takes about ten years to get used to how old you are.,Unknown Author",
		"What if this weren't a hypothetical question?,Unknown Author",
		"The fact that no one understands you doesn't mean you're an artist.,Unknown Author",
		"Everyone is entitled to be stupid, but some abuse the privilege.,Unknown Author",
		"Laughter is the shortest distance between two people.,Victor Borge",
		"You miss 100 percent of the shots you never take.,Wayne Gretzky",
		"Research is what I'm doing when I don't know what I'm doing.,Wernher von Braun",
		"The trouble with being poor is that it takes up all of your time.,Willem de Kooning",
		"It is easier to forgive an enemy than to forgive a friend.,William Blake",
		"It is impossible to defeat an ignorant man in argument.,William G. McAdoo",
		"The future is here. It's just not widely distributed yet.,William Gibson",
		"I feel like a fugitive from the law of averages.,William H. Mauldin",
		"I respect faith, but doubt is what gets you an education.,Wilson Mizner",
		"No one goes there nowadays, it's too crowded.,Yogi Berra",
		"When I read about the evils of drinking, I gave up reading.,Henny Youngman",
		"Seriousness is the only refuge of the shallow.,Oscar Wilde",
		"A conclusion is the place where you got tired thinking.,Martin H. Fischer",
		"Never believe anything until it has been officially denied.,Claud Cockburn",
		"First we make our habits, then our habits make us.,Charles C. Noble",
		"I never apologize. I'm sorry, but that's just the way I am.,Homer Simpson",
		"Who looks outside, dreams; who looks inside, awakes.,Carl Jung",
		"Eighty percent of success is showing up.,Woody Allen",
		"'Someday' is the day after you die.,Steve Pavlina",
		"Time flies like an arrow; fruit flies like a banana.,Groucho Marx",
		"You will not be punished for your anger, you will be punished by your anger.,Buddha",
		"Who is wise? He who learns from all men.,Ben Zoma",
		"When a thing is funny, search it carefully for a hidden truth.,George Bernard Shaw",
		"Tomorrow is often the busiest day of the week.,Spanish Proverb",
		"You can never get enough of what you don't really need.,Eric Hoffer",
		"I'd rather be hated for who I am, than loved for who I am not.,Kurt Cobain",
		"Practice doesn't make perfect. Perfect practice makes perfect.,Vince Lombardi",
		"Never judge a book by its movie.,J. W. Eagan",
		"The more things a man is ashamed of, the more respectable he is.,George Bernard Shaw",
		"A man is but the product of his thoughts; what he thinks, he becomes.,Gandhi",
		"Time is the stuff life is made of.,Benjamin Franklin",
		"Even a stopped clock is right twice a day. ,Marie von Ebner-Eschenbach",
		"Time you enjoy wasting is not wasted.,John Lennon",
		"Silence is so accurate.,Mark Rothko",
		"Quiet is the absence of sound. Silence is the presence of silence.,Robert Fripp",
		"Love is the triumph of imagination over intelligence.,H. L. Mencken",
		"I don't care what is written about me so long as it isn't true.,Dorothy Parker",
		"The bad news is time flies. The good news is you're the pilot.,Michael Althsuler"
};
void scroll_quote(int pixels_to_scroll_by);

void anim_stopped_handler(Animation *animation, bool finished, void *context) {
	property_animation_destroy(scroll_animation);
	scroll_quote(pixels_to_scroll);
}

void scroll_quote(int pixels_to_scroll) {
	//to keep time layer always on top
	layer_remove_from_parent(text_layer_get_layer(time_layer));
	layer_add_child(window_get_root_layer(main_window), text_layer_get_layer(time_layer));

	//from...
	GRect start_frame = GRect(2, QUOTE_Y_BEGIN, SCREENWIDTH, AUTOSCROLLHEIGHT);
	//to...
	GRect finish_frame =  GRect(2, QUOTE_Y_BEGIN+pixels_to_scroll -2, SCREENWIDTH, AUTOSCROLLHEIGHT);

	scroll_animation = property_animation_create_layer_frame(
			text_layer_get_layer(text_layer_quote), &start_frame, &finish_frame);
	animation_set_handlers((Animation*)scroll_animation, (AnimationHandlers) {
		.stopped = anim_stopped_handler
	}, NULL);

	animation_set_duration((Animation*)scroll_animation,-100* pixels_to_scroll);
	animation_set_curve((Animation*)scroll_animation, AnimationCurveLinear);
	animation_set_delay((Animation*)scroll_animation, 3000);

	animation_schedule((Animation*)scroll_animation);
}

static int already_quoted(int day){
	if (persist_exists(key_daily_quote)) {
		if(day == persist_read_int(key_daily_quote)){
			return 1;
		}
	}
	persist_write_int(key_daily_quote, day);
	return 0;
}

static void update_quote(int idx_quote) {
	int idx_quote_rand_pick;
	property_animation_destroy(scroll_animation);
	text_layer_set_text(text_layer_quote,"");

	if (idx_quote == -1){
		idx_quote_rand_pick = rand()%(sizeof(QUOTES)/sizeof(char*));
		text_layer_set_text(text_layer_quote, QUOTES[idx_quote_rand_pick]);
		persist_write_int(key_idx_daily_quote, idx_quote_rand_pick);
	}
	else{
		text_layer_set_text(text_layer_quote, QUOTES[idx_quote]);

	}

	GSize text_size = text_layer_get_content_size(text_layer_quote);
	pixels_to_scroll = SCREENHEIGHT - QUOTE_Y_BEGIN - text_size.h;



	if (pixels_to_scroll < 0) {
		//there are some pixels outside of the screen, therefore start scrolling
		scroll_quote(pixels_to_scroll);
	}
}




static void update_time() {

	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);

	static char s_buffer[8];
	strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
			"%H:%M" : "%I:%M", tick_time);

	text_layer_set_text(time_layer, s_buffer);


}





static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
	if (already_quoted(tick_time->tm_yday)==0){
		update_quote(-1);
	}
	else{
		update_quote(persist_read_int(key_idx_daily_quote));
	}

}


static void main_window_load(Window *window) {

	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);

	time_layer = text_layer_create(GRect(0, 0, bounds.size.w, QUOTE_Y_BEGIN));


	text_layer_set_background_color(time_layer, GColorWhite);
	text_layer_set_text_color(time_layer, GColorBlack);
	text_layer_set_text(time_layer, "--:--");
	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);


	text_layer_quote = text_layer_create(GRect(2, QUOTE_Y_BEGIN, SCREENWIDTH, AUTOSCROLLHEIGHT));
	text_layer_set_font(text_layer_quote, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

	layer_add_child(window_layer, text_layer_get_layer(time_layer));
	layer_add_child(window_layer, text_layer_get_layer(text_layer_quote));
}

static void main_window_unload(Window *window) {
	text_layer_destroy(time_layer);
	text_layer_destroy(text_layer_quote);
	animation_schedule((Animation*)scroll_animation);
}

static void init() {
	main_window = window_create();

	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	window_set_window_handlers(main_window, (WindowHandlers) {
		.load = main_window_load,
				.unload = main_window_unload,
	});
	window_stack_push(main_window, true);
}

static void deinit() {
	window_destroy(main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}

